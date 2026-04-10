/**
 * Description: Counts the number of times each line appears in the file.  Input can be a filename (first arg) or stdin.
 *              Names > 30 characters trigger a warning.  Empty lines are skipped with a warning.
 *              Output is a series of NameCountData structs written to a memory-mapped file named {PID}.out.
 *              Stderr is redirected to PID.err, where PID is the current process id.
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 3/24/26
 * Creation date: 2/11/26
 **/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define BUFFER_SIZE 1024 // buffer size for reading lines
#define MAX_NAME_LENGTH 30 // max name length
#define MAX_NAMES 100

typedef struct entry { // linked list for storing the names and counts
   char name[MAX_NAME_LENGTH + 1];
   int count;
   struct entry *next;
} ENTRY;

typedef struct {
    char name[MAX_NAME_LENGTH + 1];
    int count;
} NameCountData;

ENTRY *nameList = NULL;  // head of list of names

void updateCount(char *name); 

/**
 * usage: countnames [filename]
 * filename is the file to read from, otherwise reads from stdin
 */

int main(int argc, char *argv[]) {
   FILE *fp; // file pointer for input stream
   char buf[BUFFER_SIZE]; // overly large read buffer to handle long lines
   int lines = 0; // current line number, only used for error messages
   char *outFileName, *errFileName;



   if (argc == 2) {
      if ((fp = fopen(argv[1], "r")) == NULL) {
         fprintf(stderr, "error: cannot open file %s\n", argv[1]);
         return 1;
      }
   }
   else if (argc == 1) { // input from stdin
      fp = stdin;
   }
   else { // invalid arguments
      printf("Usage: %s [filename]\n", argv[0]);
      return 1;
   }

   // redirect stderr to #.err
   asprintf(&errFileName, "%ld.err", (long)getpid());
   if (dup2(open(errFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666), STDERR_FILENO) == -1) {
      fprintf(stderr, "Error redirecting stderr to %s\n", errFileName);
   }
   free(errFileName); // free the buffer returned by asprintf

   while (fgets(buf, sizeof(buf), fp)) { // read input until empty and add to list
      lines++;
      buf[strcspn(buf, "\n")] = 0;  // fix trailing newline
      if (strlen(buf) == 0) {       // skip empty lines
         fprintf(stderr, "Warning - Line %d is empty.\n", lines);
         continue;
      }
      if (strlen(buf) > MAX_NAME_LENGTH) { // long name
         fprintf(stderr, "Warning - Line %d name length greater than %d\n", lines, MAX_NAME_LENGTH);
      }

      updateCount(buf);
   }

   // Write namecount 
   asprintf(&outFileName, "%ld.out", (long)getpid());
   int fd = open(outFileName, O_RDWR | O_CREAT | O_TRUNC, 0666);
   free(outFileName);
   ftruncate(fd, MAX_NAMES * sizeof(NameCountData));
   NameCountData *output = mmap(NULL, MAX_NAMES * sizeof(NameCountData),PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   close(fd);  

   int i = 0;
   ENTRY *cur = nameList;
   while (cur && i < MAX_NAMES - 1) {
      strncpy(output[i].name, cur->name, MAX_NAME_LENGTH);
      output[i].count = cur->count;
      i++;
      cur = cur->next;
   }

   munmap(output, MAX_NAMES * sizeof(NameCountData));
   fclose(fp);
   return 0;
}

/**
 * updateCount - Maintains a linked list of names and counts.
 * Assumption: nameList is a pointer to the head of the linked list containing the names/counts.
 *             Name is null terminated and not empty.
 * Input parameters: name1 - the name to add to the list or update the count for
 * Returns: none
 */
void updateCount(char *name) {
   ENTRY *newName;         // new entry
   ENTRY *cur = nameList;  // current entry for traversal


   while (cur) {
      if (strcmp(cur->name, name) == 0) {  // name in list
         cur->count++;
         return;
      }
      cur = cur->next;
   }

   // Add new name
   if (!(newName = malloc(sizeof(ENTRY)))) {
      fprintf(stderr, "error can't malloc new entry\n");
      exit(1);
   }
   strncpy(newName->name, name, MAX_NAME_LENGTH);
   newName->count = 1;
   newName->next = nameList;  // Insert at the beginning
   nameList = newName;

}
