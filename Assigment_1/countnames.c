#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 31 // 30 Chars + '\0'
#define NUM_NAMES 101  // 100 names + ' '

int main(int argc, char *argv[]) {
  int counts[NUM_NAMES] = {0};
  char names[NUM_NAMES][NAME_LENGTH];

  // determine if file name is given or stdin should be read directly (argc == 2
  // or 1)

  FILE *fp;

  if (argc == 1) {
    fp = stdin;
  } else if (argc == 2) {
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
      printf("cannot open file\n");
      exit(1);
    }
  } else {
    printf("Too many arguments");
    exit(1);
  }

  // loop through lines (how does the last line work?)

  char buffer[NAME_LENGTH + 1]; // max size of a name plus 1 for \n

  for (int i = 1; fgets(buffer, sizeof(buffer), fp) != NULL; i++) {
    buffer[strcspn(buffer, "\n")] = '\0';
    // remove trailing newline character
    // discard line if empty (emmit warning)
    if (strlen(buffer) < 1) {
      fprintf(stderr, "Warning - Line %d is empty.\n", i);
      continue;
    }
    // printf("Line %d: %s\n", i, buffer);
    // loop through names comparing with the one read in
    for (int j = 0; j < NUM_NAMES; j++) { // break out of code internally
      if (counts[j] == 0) {               // name not seen before
        strcpy(names[j], buffer);
        counts[j] = 1;
        break;
      }
      if (strcmp(names[j], buffer) == 0) {
        counts[j]++;
        break;
      }
    }
    if (counts[NUM_NAMES - 1] != 0) {
      fprintf(stderr,
              "Warning: Buffer is full. Unable to read more names starting at "
              "line %d\n",
              i);
    }
  }

  // print names and counts
  for (int i = 0; i < NUM_NAMES; i++) {
    if (counts[i] > 0) {
      printf("%s: %d\n", names[i], counts[i]);
    } else {
      break;
    }
  }

  return 0;
}