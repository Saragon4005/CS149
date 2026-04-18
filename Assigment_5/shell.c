/**
 * Description: Parent shell process. Reads command lines from stdin in the format:
 *              <binary> <file1> <file2> ... <fileN>
 *              Forks one child per file. Each child (countnames) writes NameCountData structs
 *              to a memory-mapped file named {PID}.out. The parent waits for all children,
 *              then maps each child's {PID}.out file and aggregates name counts into a
 *              dynamically allocated hash table (NameCountData **).  The hash table starts
 *              at 10 buckets (malloc) and doubles via realloc when the load factor exceeds 2.
 *              Names are stored as heap strings using strdup; collisions are resolved by
 *              chaining via the next pointer.  All malloc'd memory is freed after each
 *              command (valgrind-clean).  Aggregated results are printed to stdout.
 *              Example: ./countnames a1.txt a2.txt a3.txt
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 4/13/26
 * Creation date: 3/4/26
 **/
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stddef.h>

#define HASH_INITIAL 10      /* initial hash size */
#define PROMPT       "%% "   /* shell prompt */

typedef struct NameCountData {
  char *name;
  int count;
  struct NameCountData *next;
} NameCountData;

static NameCountData **hashTable = NULL;
static int tableSize = 0;
static int totalEntries = 0;

/* hash generator, originally thought this was djb2, but not quite right? */
static unsigned int hashFn(const char *name, int size) {
  unsigned int h = 0;
  while (*name) h = h * 31 + (unsigned char)*name++;
  return h % (unsigned int)size;
}

/* hash init */
static void initTable(void) {
  tableSize = HASH_INITIAL;
  hashTable = malloc(tableSize * sizeof(NameCountData *));
  if (!hashTable) { perror("malloc"); exit(1); }
  memset(hashTable, 0, tableSize * sizeof(NameCountData *));
  totalEntries = 0;
}

/* grow and rehash the table */
static void rehash(void) {
  int newSize = tableSize * 2;

  /* Collect all existing entry pointers before touching the table */
  NameCountData **all = malloc(totalEntries * sizeof(NameCountData *));
  if (!all) { perror("malloc"); exit(1); }
  int n = 0;
  for (int i = 0; i < tableSize; i++) {
    NameCountData *e = hashTable[i];
    while (e) { all[n++] = e; e = e->next; }
  }

  /* grow hash table */
  hashTable = realloc(hashTable, newSize * sizeof(NameCountData *));
  if (!hashTable) { perror("realloc"); exit(1); }
  memset(hashTable, 0, newSize * sizeof(NameCountData *));
  tableSize = newSize;

  /* finally rehash */
  for (int i = 0; i < n; i++) {
    NameCountData *entry = all[i];
    unsigned int idx = hashFn(entry->name, tableSize);
    entry->next = hashTable[idx];
    hashTable[idx] = entry;
  }
  free(all);
}

/* insert/update entry */
static void insertName(const char *name, int count) {
  unsigned int idx = hashFn(name, tableSize);

  NameCountData *cur = hashTable[idx];
  while (cur) {    /* Search existing chain for this name */
    if (strcmp(cur->name, name) == 0) {
      cur->count += count;
      return;
    }
    cur = cur->next;
  }

  /* new */
  NameCountData *entry = malloc(sizeof(NameCountData));
  if (!entry) { perror("malloc"); exit(1); }
  entry->name = strdup(name);
  if (!entry->name) { perror("strdup"); exit(1); }
  entry->count = count;
  entry->next = hashTable[idx];
  hashTable[idx] = entry;
  totalEntries++;

  /* rehash at load factor of 2 */
  if (totalEntries > tableSize * 2) {
    rehash();
  }
}

/* Print entries in table */
static void printTable(void) {
  for (int i = 0; i < tableSize; i++) {
    NameCountData *cur = hashTable[i];
    while (cur) {
      printf("%s: %d\n", cur->name, cur->count);
      cur = cur->next;
    }
  }
}

/* free all the things in the table */
static void freeTable(void) {
  for (int i = 0; i < tableSize; i++) {
    NameCountData *cur = hashTable[i];
    while (cur) {
      NameCountData *next = cur->next;
      free(cur->name);  /* free strdup'd string */
      free(cur);
      cur = next;
    }
    hashTable[i] = NULL;
  }
  free(hashTable);
  hashTable = NULL;
  tableSize = 0;
  totalEntries = 0;
}

int main(void) {
  char *buf = NULL;
  size_t buflen = 0;
  ssize_t nread;
  pid_t pid;
  int status;
  int num_children;

  printf(PROMPT);
  while ((nread = getline(&buf, &buflen, stdin)) != -1) {
    if (nread > 0 && buf[nread - 1] == '\n')
      buf[nread - 1] = '\0';

    initTable();
    num_children = 0;

    int pids_capacity = 8;
    pid_t *pids = malloc(pids_capacity * sizeof(pid_t));
    if (!pids) { perror("malloc"); exit(1); }

    char *bin = strtok(buf, " ");  /* first token is binary name */

    char *filename;
    while ((filename = strtok(NULL, " ")) != NULL) {
      if (num_children == pids_capacity) {
        pids_capacity *= 2;
        pids = realloc(pids, pids_capacity * sizeof(pid_t));
        if (!pids) { perror("realloc"); exit(1); }
      }

      if ((pid = fork()) < 0) {
        perror("fork error");
        exit(1);
      }
      else if (pid == 0) {          /* child */
        execlp(bin, bin, filename, (char *)0);
        fprintf(stderr, "couldn't execute: %s\n", bin);
        exit(127);
      }
      else {                        /* parent */
        pids[num_children++] = pid;
      }
    }

    /* Wait for each child, aggregate into hash table */
    int remaining = num_children;
    while (remaining > 0) {
      pid_t finished = wait(&status);
      if (finished == -1) break;
      remaining--;

      char *childFile;
      asprintf(&childFile, "%ld.out", (long)finished);
      int fd = open(childFile, O_RDONLY);
      free(childFile);
      if (fd < 0) continue;

      struct stat st;
      if (fstat(fd, &st) < 0 || st.st_size == 0) { close(fd); continue; }
      size_t regionSize = (size_t)st.st_size;

      char *fileMap = mmap(NULL, regionSize, PROT_READ, MAP_SHARED, fd, 0);
      close(fd);
      if (fileMap == MAP_FAILED) continue;

      /* Parse variable-length records: [uint32_t namelen][name bytes][int count] */
      char *ptr = fileMap;
      char *end = fileMap + regionSize;
      while (ptr + sizeof(uint32_t) <= end) {
        uint32_t namelen;
        memcpy(&namelen, ptr, sizeof(uint32_t));
        ptr += sizeof(uint32_t);
        if (namelen == 0) break;  /* sentinel */
        if (ptr + namelen + sizeof(int) > end) break;  /* safety */
        char *name = malloc(namelen + 1);
        if (!name) { perror("malloc"); exit(1); }
        memcpy(name, ptr, namelen);
        name[namelen] = '\0';
        ptr += namelen;
        int count;
        memcpy(&count, ptr, sizeof(int));
        ptr += sizeof(int);
        insertName(name, count);
        free(name);
      }

      munmap(fileMap, regionSize);
    }

    // print results and clean up
    printTable(); 
    freeTable();
    free(pids);

    printf(PROMPT);
  }
  free(buf);
  exit(0);
}
