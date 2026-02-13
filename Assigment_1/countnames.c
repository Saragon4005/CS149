#include <stdio.h>

#define NAME_LENGTH 31  // 30 Chars + '\0'
#define NUM_NAMES 101   // 100 names + ' '

int main(int argc, char* argv[]) {
  int counts[NUM_NAMES];
  char names[NUM_NAMES][NAME_LENGTH];

  // determine if file name is given or stdin should be read directly (argc == 2
  // or 1)
  if (argc == 1) {
    /* code */
  }

  // loop through lines (how does the last line work?)
  // discard line if empty (emmit warning)
  // loop through names comparing with the one read in
  // some mechanism to check if its not been seen (check for default? or
  // counter)

  printf("Hello, World!");
  return 0;
}