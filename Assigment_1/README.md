# Assignment 1: countnames

**Student Name:** Raven Keszei

## Overview

This assignment implements a utility program `countnames` that reads a list of names from a file or stdin and counts the occurrences of each unique name.

## Compilation

To compile the program with warnings and error checking:

```bash
gcc -o countnames countnames.c -Wall -Werror
```

## Test Cases

### Test Case 1: `test/names_empty_lines.txt`

**How to Run:**

```bash
./countnames test/names_empty_lines.txt
```

**Justification:** Tests handling of empty lines in the input file. Verifies that the program correctly warns about empty lines to stderr, ignores them in counting, and properly counts duplicate names.

---

### Test Case 2: `test/names_single.txt`

**How to Run:**

```bash
./countnames test/names_single.txt
```

**Justification:** Tests basic functionality with a minimal single-entry file. Ensures the program handles names with spaces correctly and works with the simplest valid input.

---

### Test Case 3: `test/names_duplicates.txt`

**How to Run:**

```bash
./countnames test/names_duplicates.txt
```

**Justification:** Tests proper counting of multiple duplicate names with varying frequencies. Verifies the core name-counting algorithm correctly tracks and counts each unique name's occurrences.

---

## Reading from stdin

You can also pipe input to the program:

```bash
cat test/names.txt | ./countnames
```

Or provide input interactively (press Ctrl+D to signal EOF):

```bash
./countnames
Enter names below (Ctrl+D to end):
Alice
Bob
Alice
^D
```

## Error Handling

If a non-existent file is passed:

```bash
./countnames nonexistent.txt
```

**Expected Output:**

```plain
error: cannot open file
```

**Exit Code:** 1

## Lessons Learned

Through this assignment, I learned:

- How to use `fgets()` to read lines from files in C and handle the trailing newline character
- How to use string manipulation functions like `strlen()`, `strcspn()`, and `strcmp()` for string handling
- How to manage arrays of strings in C
- The distinction between reading from files via command-line arguments (argc/argv) versus reading from stdin
- Proper file I/O error handling with `fopen()` and NULL pointer checks
- How to direct output to stdout and stderr appropriately using `printf()` and `fprintf()`

## References

- **fgets() man page:** `man fgets` (or <https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm>)
- **GNU C Library Reference Manual:** <https://www.gnu.org/software/libc/manual/>

## Acknowledgements

I used VS Code's inline hints and Google to learn the syntax of C arrays, string handling and I/O management.
