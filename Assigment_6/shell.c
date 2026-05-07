/**
 * Description: A6 Multi-threaded shell process.
 *              Reads command lines from stdin in the format:
 *                <binary> <file1> <file2> ... <fileN>
 *              The first token is the old binary name and
 *              is ignored (kept for command-line compatibility with A1–A5).
 *              For each filename, spawns one pthread (countNamesThread,
 *              from namecount.c) that reads the file and inserts each
 *              name into the hash table. When all the threads are done, the main thread prints the aggregated counts to stdout.
 *              The namecount code/hash functions are in namecount.c/h and must be linked in.
 *
 * Build:       gcc -D_REENTRANT -pthread -Wall -Werror -o shell shell.c namecount.c
 *              (or make shell or just make all)
 * Run:           $ ./shell
 *                % ./countnames names1.txt names2.txt
 *                Tom Wu: 4
 *                Jenn Xu: 2
 *                %
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 5/4/26
 * Creation date: 3/4/26
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "namecount.h"

#define PROMPT         "%% "   /* shell prompt (printf renders as "% ") */
#define TIDS_CAPACITY  8       /* initial tids array size; doubles on overflow */

int main(void) {
  char *buf = NULL;
  size_t buflen = 0;
  ssize_t nread;

  printf(PROMPT);
  while ((nread = getline(&buf, &buflen, stdin)) != -1) { 
    if (nread > 0 && buf[nread - 1] == '\n') // newline scrub
      buf[nread - 1] = '\0';

    initTable();

    int tids_capacity = TIDS_CAPACITY;
    int num_threads = 0;
    pthread_t *tids = malloc(tids_capacity * sizeof(pthread_t));
    if (!tids) { perror("malloc"); exit(1); }

    (void)strtok(buf, " \t");  /* discard legacy binary name for compatibility */

    char *filename;
    while ((filename = strtok(NULL, " \t")) != NULL) { // loop over filenames
      if (num_threads == tids_capacity) { // grow the tid array if needed
        tids_capacity *= 2;
        tids = realloc(tids, tids_capacity * sizeof(pthread_t));
        if (!tids) { perror("realloc"); exit(1); }
      }

      int rc = pthread_create(&tids[num_threads], NULL, countNamesThread, filename); // thread for each filename
      if (rc != 0) {
        fprintf(stderr, "pthread_create failed for %s\n", filename);
        continue;
      }
      num_threads++;
    }

    for (int i = 0; i < num_threads; i++) pthread_join(tids[i], NULL); // join all

    printTable();
    freeTable();
    free(tids);

    printf(PROMPT);
  }
  free(buf);
  return 0;
}
