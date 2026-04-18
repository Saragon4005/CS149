/**
 * Description: Counts the number of times each unique name (line) appears in a file.
 *              Input can be a filename (first arg) or stdin.
 *              Names longer than 30 characters trigger a warning to stderr; empty lines
 *              are skipped with a warning.  Names are stored in a dynamically allocated
 *              linked list using strdup (no fixed-size limit).  Lines are read with
 *              getline so the read buffer grows automatically for long input.
 *              Results are written as fixed-size NameCountData structs to a memory-mapped
 *              file named {PID}.out for the parent to read.  Stderr is redirected to
 *              {PID}.err.  All heap memory is freed before exit (valgrind-clean).
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 4/13/26
 * Creation date: 2/11/26
 **/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct entry {
   char *name;
   int count;
   struct entry *next;
} ENTRY;

static ENTRY *nameList = NULL;  /* name count list */

void updateCount(char *name);

int main(int argc, char *argv[]) {
   FILE *fp;
   int lines = 0;
   char *outFileName, *errFileName;

   if (argc == 2) {
      if ((fp = fopen(argv[1], "r")) == NULL) {
         fprintf(stderr, "error: cannot open file %s\n", argv[1]);
         return 1;
      }
   }
   else if (argc == 1) {
      fp = stdin;
   }
   else {
      printf("Usage: %s [filename]\n", argv[0]);
      return 1;
   }

   /* Redirect stderr to {PID}.err for warnings */
   asprintf(&errFileName, "%ld.err", (long)getpid());
   if (dup2(open(errFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666), STDERR_FILENO) == -1)
      fprintf(stderr, "Error redirecting stderr to %s\n", errFileName);
   free(errFileName);

   /* Read lines with getline */
   char *line = NULL;
   size_t len = 0;
   ssize_t nread;

   while ((nread = getline(&line, &len, fp)) != -1) {
      lines++;
      /* Strip trailing newline */
      if (nread > 0 && line[nread - 1] == '\n')
         line[--nread] = '\0';

      if (nread == 0) {
         fprintf(stderr, "Warning - Line %d is empty.\n", lines);
         continue;
      }
      updateCount(line);
   }
   free(line);  /* free the getline buffer */
   /* Calculate exact bytes needed */
   size_t mmapSize = sizeof(uint32_t);  /* sentinel */
   ENTRY *cur = nameList;
   while (cur) {
      mmapSize += sizeof(uint32_t) + strlen(cur->name) + sizeof(int);
      cur = cur->next;
   }

   /* Write name counts to PID.out */
   asprintf(&outFileName, "%ld.out", (long)getpid());
   int fd = open(outFileName, O_RDWR | O_CREAT | O_TRUNC, 0666);
   free(outFileName);
   ftruncate(fd, mmapSize);
   char *output = mmap(NULL, mmapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   close(fd);

   char *ptr = output;
   cur = nameList;
   while (cur) {
      uint32_t namelen = (uint32_t)strlen(cur->name);
      memcpy(ptr, &namelen, sizeof(uint32_t)); ptr += sizeof(uint32_t);
      memcpy(ptr, cur->name, namelen);          ptr += namelen;
      memcpy(ptr, &cur->count, sizeof(int));    ptr += sizeof(int);
      cur = cur->next;
   }

   uint32_t zero = 0;
   memcpy(ptr, &zero, sizeof(uint32_t));  // end of the record

   munmap(output, mmapSize);
   fclose(fp);

   /* clean up ll memory */
   cur = nameList;
   while (cur) {
      ENTRY *next = cur->next;
      free(cur->name);  /* because strdup() */
      free(cur);
      cur = next;
   }
   nameList = NULL;

   return 0;
}

/**
 * updateCount - Finds the name in the list and increments its count, or adds a new entry.
 * Assumption: nameList is the head of the linked list; name is non-empty and null-terminated.
 * Input: name - the name string to count (borrowed; stored via strdup)
 * Returns: none
 */
void updateCount(char *name) {
   ENTRY *cur = nameList;
   while (cur) {
      if (strcmp(cur->name, name) == 0) {
         cur->count++;
         return;
      }
      cur = cur->next;
   }

   /* Name not found: allocate a new node and strdup the name */
   ENTRY *newName = malloc(sizeof(ENTRY));
   if (!newName) { fprintf(stderr, "error can't malloc new entry\n"); exit(1); }
   newName->name = strdup(name);
   if (!newName->name) { perror("strdup"); exit(1); }
   newName->count = 1;
   newName->next = nameList;  /* insert at head */
   nameList = newName;
}
