#include <stdio.h>
#include <stdlib.h>

#define NAME_LENGTH 31  // 30 Chars + '\0'
#define NUM_NAMES 101   // 100 names + ' '

int main(int argc, char* argv[]) {
  int counts[NUM_NAMES] = {0};
  char names[NUM_NAMES][NAME_LENGTH];

  // determine if file name is given or stdin should be read directly (argc == 2
  // or 1)

  FILE* fp;

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

  char buffer[NAME_LENGTH];

  while (fgets(buffer, NAME_LENGTH, fp) != NULL) {
    printf(buffer);
  }

  // discard line if empty (emmit warning)
  // loop through names comparing with the one read in
  // some mechanism to check if its not been seen (check for default? or
  // counter)

  return 0;
}