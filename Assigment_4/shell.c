/**
 * Description: Parent shell process. Reads command lines from stdin in the
 * format: <binary> <file1> <file2> ... <fileN> Forks one child per file. Each
 * child (countnames) writes NameCountData structs to a memory-mapped file named
 * {PID}.out. The parent waits for all children to finish, then opens each
 * child's {PID}.out file, maps it into memory, aggregates name counts across
 * all children, and prints the combined results. Example: countnames a1.txt
 * a2.txt a3.txt Author names: Steve Rubin, Raven Keszei Author emails:
 * steven.rubin@sjsu.edu, raven.keszei@sjsu.edu Last modified date: 3/24/26
 * Creation date: 3/4/26
 **/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 1024
#define MAX_NAME_LENGTH 30
#define MAX_CHILDREN 64
#define MAX_NAMES 100

typedef struct {
  char name[MAX_NAME_LENGTH + 1];
  int count;
} NameCountData;

typedef struct name_count_entry {
  char name[MAX_NAME_LENGTH + 1];
  int count;
  struct name_count_entry *next;
} NameCountEntry;

NameCountEntry *nameCountList = NULL;

/**
 * name_count - Maintains a linked list of names and counts.
 * Assumption: nameCountList is a pointer to the head of the linked list
 * containing the names/counts. Input parameters: data - the NameCountData
 * struct containing the name and count to add/update to the list Returns: none
 */

void name_count(NameCountData *data) {
  NameCountEntry *entry;
  NameCountEntry *cur = nameCountList;

  while (cur) {
    if (strcmp(cur->name, data->name) == 0) {
      cur->count += data->count;
      return;
    }
    cur = cur->next;
  }

  if (!(entry = malloc(sizeof(NameCountEntry)))) {
    fprintf(stderr, "error can't malloc new entry\n");
    exit(1);
  }
  strncpy(entry->name, data->name, MAX_NAME_LENGTH);
  entry->name[MAX_NAME_LENGTH] = '\0';
  entry->count = data->count;
  entry->next = nameCountList;
  nameCountList = entry;
}

int main(void) {
  char buf[MAXLINE];
  pid_t pid;
  int status;
  int num_children;

  printf("%% ");
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = 0; /* replace newline with null */

    // free aggregation list from previous command
    NameCountEntry *cur = nameCountList;
    while (cur) {
      NameCountEntry *next = cur->next;
      free(cur);
      cur = next;
    }
    nameCountList = NULL;
    num_children = 0;

    // Setup mmap for this command
    size_t regionSize = MAX_NAMES * sizeof(NameCountData);
    size_t totalSize = (MAX_CHILDREN + 1) * regionSize;
    void *GLOBAL = mmap(NULL, totalSize, PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (GLOBAL == MAP_FAILED) {
      perror("mmap");
      exit(1);
    }
    memset(GLOBAL, 0, totalSize);

    pid_t pids[MAX_CHILDREN];

    char *bin = strtok(buf, " "); // extract binary name

    char *filename;
    while ((filename = strtok(NULL, " ")) != NULL) {
      if (num_children >= MAX_CHILDREN) {
        fprintf(stderr, "too many files (max %d)\n", MAX_CHILDREN);
        break;
      }

      if ((pid = fork()) < 0) {
        perror("fork error");
        exit(1);
      } else if (pid == 0) { /* child */
        execlp(bin, bin, filename, (char *)0);
        fprintf(stderr, "couldn't execute: %s\n", bin);
        exit(127);
      } else {                      /* parent */
        pids[num_children++] = pid; // remember pid
      }
    }

    /* wait for all children and map their output files */
    int remaining = num_children;
    while (remaining > 0) {
      pid_t finished = wait(&status);
      if (finished == -1)
        break;
      remaining--;

      // Find child index
      int childIdx = -1;
      for (int i = 0; i < num_children; i++) {
        if (pids[i] == finished) {
          childIdx = i;
          break;
        }
      }
      if (childIdx == -1)
        continue;

      // Map child's PID.out into its GLOBAL subregion
      char *childFile;
      asprintf(&childFile, "%ld.out", (long)finished);
      int fd = open(childFile, O_RDONLY);
      free(childFile);
      if (fd < 0)
        continue;

      NameCountData *fileMap =
          mmap(NULL, regionSize, PROT_READ, MAP_SHARED, fd, 0);
      close(fd);
      if (fileMap == MAP_FAILED)
        continue;

      NameCountData *childRegion =
          (NameCountData *)((char *)GLOBAL + childIdx * regionSize);
      memcpy(childRegion, fileMap, regionSize);
      munmap(fileMap, regionSize);
    }

    /* aggregate child regions into linked list */
    for (int i = 0; i < num_children; i++) {
      NameCountData *region =
          (NameCountData *)((char *)GLOBAL + i * regionSize);
      for (int j = 0; j < MAX_NAMES && region[j].name[0] != '\0'; j++) {
        name_count(&region[j]);
      }
    }

    /* write linked list into summation region */
    NameCountData *summation =
        (NameCountData *)((char *)GLOBAL + num_children * regionSize);
    int sumIdx = 0;
    NameCountEntry *entry = nameCountList;
    while (entry && sumIdx < MAX_NAMES - 1) {
      strncpy(summation[sumIdx].name, entry->name, MAX_NAME_LENGTH);
      summation[sumIdx].count = entry->count;
      sumIdx++;
      entry = entry->next;
    }
    // summation[sumIdx].name[0] == '\0' — already zero-filled

    /* print results from summation region */
    for (int i = 0; summation[i].name[0] != '\0'; i++) {
      printf("%s: %d\n", summation[i].name, summation[i].count);
    }

    /* cleanup */
    munmap(GLOBAL, totalSize);

    printf("%% ");
  }
  exit(0);
}
