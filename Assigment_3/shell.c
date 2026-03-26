/**
 * Description: Parent shell process. Reads command lines from stdin in the
 * format: <binary> <file1> <file2> ... <fileN> Forks one child per file,
 * passing a pipe write-end via stdout redirect. Each child (countnames) writes
 * NameCountData structs to the pipe. The parent reads all pipes, aggregates
 * name counts across children, and prints the combined results. Example:
 * countnames a1.txt a2.txt a3.txt Author names: Steve Rubin, Raven Keszei
 * Author emails: steven.rubin@sjsu.edu, raven.keszei@sjsu.edu
 * Last modified date: 3/24/26
 * Creation date: 3/4/26
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 1024
#define MAX_NAME_LENGTH 30
#define MAX_CHILDREN 64

typedef struct {
  char name[MAX_NAME_LENGTH];
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
  int readFds[MAX_CHILDREN];
  int num_children;

  printf("%% "); /* print prompt (printf requires %% to print %) */
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

    char *bin = strtok(buf, " "); // extract binary name

    char *filename;
    while ((filename = strtok(NULL, " ")) != NULL) {
      if (num_children >= MAX_CHILDREN) {
        fprintf(stderr, "too many files (max %d)\n", MAX_CHILDREN);
        break;
      }

      int pipefd[2];
      if (pipe(pipefd) < 0) {
        perror("pipe error");
        exit(1);
      }

      if ((pid = fork()) < 0) {
        perror("fork error");
        exit(1);
      } else if (pid == 0) { /* child */
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO); // stdout to pipe write end
        close(pipefd[1]);
        execlp(bin, bin, filename, (char *)0);
        fprintf(stderr, "couldn't execute: %s\n", bin);
        exit(127);
      } else { /* parent */
        close(pipefd[1]);
        readFds[num_children++] = pipefd[0];
      }
    }

    /* parent: read from all pipes */
    for (int i = 0; i < num_children; i++) {
      NameCountData data;
      while (read(readFds[i], &data, sizeof(NameCountData)) ==
             sizeof(NameCountData)) {
        name_count(&data);
      }
      close(readFds[i]);
    }

    /* wait for all children */
    while (wait(&status) != -1)
      ;

    /* print results */
    NameCountEntry *entry = nameCountList;
    while (entry) {
      printf("%s: %d\n", entry->name, entry->count);
      entry = entry->next;
    }

    printf("%% ");
  }
  exit(0);
}
