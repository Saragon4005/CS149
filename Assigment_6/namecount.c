/**
 * Description: Shared name-count hash table and per-file thread worker.
 *              Linked into both shell and countnames so the two binaries
 *              share one implementation of the table, mutex, and worker.
 *              The table starts at HASH_INITIAL buckets and doubles via realloc when
 *              the load factor exceeds LOAD_FACTOR_MAX.  Names are heap strings (strdup);
 *              collisions chain via the next pointer.  insertName acquires
 *              hashLock for the entire insert (including lookup, insert, and possible rehash).
 *              for thread safety.  printTable and freeTable are not thread-safe and should only be 
 *              called by the main thread after all workers have joined.
 * Build:       Not built standalone; must be linked into shell and countnames.  For example:
 *                gcc -D_REENTRANT -pthread -Wall -Werror -o shell      shell.c      namecount.c
 *                gcc -D_REENTRANT -pthread -Wall -Werror -o countnames countnames.c namecount.c
 *              (or just make all)
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 5/4/26
 * Creation date: 5/4/26
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "namecount.h"

#define HASH_INITIAL    10   /* initial bucket count */
#define LOAD_FACTOR_MAX 2    /* hash table load factor */

typedef struct NameCountData {
   char *name;
   int count;
   struct NameCountData *next;
} NameCountData;

static NameCountData **hashTable = NULL;
static int tableSize = 0;
static int totalEntries = 0;
static pthread_mutex_t hashLock = PTHREAD_MUTEX_INITIALIZER;

/* hash generator (rolling 31-multiplier, similar to djb2) */
static unsigned int hashFn(const char *name, int size) {
   unsigned int h = 0;
   while (*name) h = h * 31 + (unsigned char)*name++;
   return h % (unsigned int)size;
}

// initialize the hash table with HASH_INITIAL buckets and zero entries
void initTable() {
   tableSize = HASH_INITIAL;
   hashTable = malloc(tableSize * sizeof(NameCountData *));
   if (!hashTable) { perror("malloc"); exit(1); }
   memset(hashTable, 0, tableSize * sizeof(NameCountData *));
   totalEntries = 0;
}

// rehash, Caller must hold hashLock. 
static void rehash() {
   int newSize = tableSize * 2;
   NameCountData **all = malloc(totalEntries * sizeof(NameCountData *));
   if (!all) { perror("malloc"); exit(1); }
   int n = 0;
   for (int i = 0; i < tableSize; i++) {
      NameCountData *e = hashTable[i];
      while (e) { all[n++] = e; e = e->next; }
   }
   hashTable = realloc(hashTable, newSize * sizeof(NameCountData *));
   if (!hashTable) { perror("realloc"); exit(1); }
   memset(hashTable, 0, newSize * sizeof(NameCountData *));
   tableSize = newSize;
   for (int i = 0; i < n; i++) {
      NameCountData *entry = all[i];
      unsigned int idx = hashFn(entry->name, tableSize);
      entry->next = hashTable[idx];
      hashTable[idx] = entry;
   }
   free(all);
}

// insert a name or increment its count; locks hashLock for the entire op 
static void insertName(const char *name) {
   pthread_mutex_lock(&hashLock);

   unsigned int idx = hashFn(name, tableSize);
   NameCountData *cur = hashTable[idx];
   while (cur) {
      if (strcmp(cur->name, name) == 0) { // increment count if found
         cur->count++;
         pthread_mutex_unlock(&hashLock);
         return;
      }
      cur = cur->next;
   }

   // add a new entry
   NameCountData *entry = malloc(sizeof(NameCountData));
   if (!entry) { perror("malloc"); exit(1); }
   entry->name = strdup(name);
   if (!entry->name) { perror("strdup"); exit(1); }
   entry->count = 1;
   entry->next = hashTable[idx];
   hashTable[idx] = entry;
   totalEntries++;

   // check load factor and grow if needed
   if (totalEntries > tableSize * LOAD_FACTOR_MAX) rehash();

   pthread_mutex_unlock(&hashLock);
}

void printTable(void) {
   for (int i = 0; i < tableSize; i++) {
      NameCountData *cur = hashTable[i];
      while (cur) {
         printf("%s: %d\n", cur->name, cur->count);
         cur = cur->next;
      }
   }
}

// free the table and its entries
void freeTable() {
   for (int i = 0; i < tableSize; i++) {
      NameCountData *cur = hashTable[i];
      while (cur) { // free each entry in the chain
         NameCountData *next = cur->next;
         free(cur->name);
         free(cur);
         cur = next;
      }
      hashTable[i] = NULL;
   }
   free(hashTable); // finally free the array
   hashTable = NULL;
   tableSize = 0;
   totalEntries = 0;
}

// reads a file line by line and inserts names into the table. 
void *countNamesThread(void *arg) {
   const char *filename = (const char *)arg;
   FILE *fp = stdin;
   if (filename) {
      fp = fopen(filename, "r");
      if (!fp) {
         fprintf(stderr, "error: cannot open file %s\n", filename);
         return NULL;
      }
   }

   char *line = NULL;
   size_t len = 0;
   ssize_t nread;
   int lineNum = 0;

   // read each line, trim newline, and insert into table
   while ((nread = getline(&line, &len, fp)) != -1) {
      lineNum++;
      if (nread > 0 && line[nread - 1] == '\n')
         line[--nread] = '\0';
      if (nread == 0) {
         fprintf(stderr, "Warning - %s line %d is empty.\n", filename ? filename : "stdin", lineNum);
         continue;
      }
      insertName(line);
   }

   free(line);
   if (fp != stdin) fclose(fp);
   return NULL;
}
