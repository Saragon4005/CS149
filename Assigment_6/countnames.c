/**
 * Description: A6 Multi-threaded name counter.  Takes one or more filenames
 *              as command-line arguments and spawns one pthread per file.
 *              Each thread reads its file and then inserts every (!empty) name
 *              into a shared hash table protected by a lock.  The
 *              main thread joins all workers, then prints aggregated counts
 *              to stdout. Non-existent files cause a per-thread error message
 *              on stderr and other threads continue. 
 * 
 *              The namecount code/hash functions are in namecount.c/h and must be linked in.

 * Build:       gcc -D_REENTRANT -pthread -Wall -Werror -o countnames countnames.c namecount.c
 *              (or make countnames OR make all)
 *                
 * Run:         $ ./countnames names1.txt names2.txt
 *              Tom Wu: 4
 *              Jenn Xu: 2
 * 
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 5/4/26
 * Creation date: 2/11/26
 **/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "namecount.h"

int main(int argc, char *argv[]) {
   if (argc < 2) {
      fprintf(stderr, "Usage: %s file1 [file2 ...]\n", argv[0]);
      return 1;
   }

   int n = argc - 1;
   initTable();

   pthread_t *tids = malloc(n * sizeof(pthread_t)); // dynamic tids list
   if (!tids) { perror("malloc"); exit(1); }

   int created = 0;
   for (int i = 0; i < n; i++) { // start thread for each file
      int rc = pthread_create(&tids[created], NULL, countNamesThread, argv[i + 1]);
      if (rc != 0) {
         fprintf(stderr, "pthread_create failed for %s\n", argv[i + 1]);
         continue;
      }
      created++;
   }

   for (int i = 0; i < created; i++) pthread_join(tids[i], NULL); // wait for all threads

   printTable();
   freeTable();
   free(tids);
   return 0;
}
