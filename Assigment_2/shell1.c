/**
 * Description: Reads a line of text containing a binary file, followed by N test files.
 *              each test file is then forked seperately and run.
 *              example input line: foo a1 a2 a3 
 *              would work 3 "foo"s with one of the a1/a2/a3 set of file names
 * Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 3/4/26
 * Creation date: 3/4/26
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 1024

int main(void) {
  char buf[MAXLINE];
  pid_t pid;
  int status;

  printf("%% "); /* print prompt (printf requires %% to print %) */
  while (fgets(buf, MAXLINE, stdin) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      buf[strlen(buf) - 1] = 0; /* replace newline with null */
    char *bin = strtok(buf, " "); // extract binary name

    char *filename;
    while ((filename = strtok(NULL, " ")) != NULL) {



      if ((pid = fork()) < 0) {
        perror("fork error");
        exit(1);
      }
      else if (pid == 0) { /* child */
        execlp(bin, bin, filename, (char *)0);
        fprintf(stderr, "couldn't execute: %s\n", buf);
        exit(127);
      }
    }
    /* parent */
    if ((pid = waitpid(pid, &status, 0)) < 0) {
      perror("waitpid error");
      exit(1);
    }
    printf("%% ");
  }
  exit(0);
}