# Assignment 5 SP26

## Objectives

* Understand memory management with C. Advance your C skills further.
* Move up to reading strings into dynamic arrays

## Background

In *Assignment 1*, you implemented the countnames utility that reads a file containing names and counts how many times each name appears.

In *Assignment 2*, you introduced *parallel processing*. The shell program spawned multiple child processes so that multiple files could be processed simultaneously.

In *Assignment 3*, child processes communicated their results back to the parent using *pipes,* so that the parent process could aggregate the results.

In *Assignment 4*, you replaced *pipe communication with shared memory using the `mmap()` system call*.

In this Assignment, you will implement dynamic memory allocation in place of static sized memory allocations (the next and last assignment will be about multi-threading in place of child processes).

The command line interface is the same as in the other assignments, in other words:
For example, running on the names?.txt files will output:
`$ ./countnames names1.txt names2.txt`

which will output to *stdout*:
`Tom Wu: 4 Jenn Xu: 2`

---

## Task

### Replace your GLOBAL mmap space from A4 (the one where the parent keeps the names counts) with an malloc'ed dynamically allocated space

You will build on your A4 implementation. The main difference is you abandon the fixed array size of the parent shell where the results get aggregated. Thus the parent can count any number of distinct names with malloc/realloc. The child processes will still store their namescounts in the same mmapped arrays as A4, but the results will be aggregated in the parent's dynamically malloc'ed array.

The struct stored in the malloc'ed space will look as follows:

```c
typedef struct {
  char *name;
  int count;
  NameCountData *next;
} NameCountData;
```

Note you use `char *` for strings, rather than char array. See the footnotes below how to use `getline` and `fgets` to get strings as string literals. After you get each string you can use `strdup` to remap that buffer to a new one (since the buffer will get overwritten when you re-call `getline` or `fgets`). You can use `strdup` to duplicate a string literal or null-terminated array of characters to another string literal (stored in Data Segment).

The `NameCountData *next` pointer is because you will use a hash table data structure, like the one provided under the [assignment 5 template code folder](https://sjsu.instructure.com/courses/1619829/assignments/7708240) (otherwise there would be little point in using threads later in A6, since locking an entire array will be slow).

So, you will read all names into a dynamically allocated array of type **NameCountData \*** for a hash table. Thus, you will abandon any previous limitations (such as, only 100 names of 30 chars each). This diagram in the slides illustrates a double pointer data structure that can be used for a hash table: [Slide Diagram](https://docs.google.com/presentation/d/1nj0bY0EKYOCzolYXlEzxSgbpw-rKMC-k7mTKBXk5qYs/edit?folder=1HuyG3ez13YNEHILs7ky31jL4dIPhU4Sp&slide=id.gab3d7a2387_0_0#slide=id.gab3d7a2387_0_0)

Initialize the array memory with `malloc` to an initial size of 10. You will use `realloc` to expand your array if the initial size turns out not to be big enough for the lines in the input. *Your implementation should work with input of up to a few hundred lines (test it with input of a hundred names).*

You must free the malloc'ed space in the end and ensure with valgrind there is no allocated memory left behind. One of the main problems in memory management with C is memory leaks. Memory leaks in an Operating System cause serious runtime problems and they can make a system unstable. You can read more about the [problems caused by memory leaks here](https://www.computerworld.com/article/2596992/memory-leaks-and-garbage-collection.html). *Ensure there are no memory leaks by freeing your memory. Use [valgrind](https://sjsu.instructure.com/courses/1619829/pages/valgrind) to detect memory leaks (valgrind is available on the zyLabs terminal).*

---

## Submission (same as A1–A4)

Besides the given test cases, you must also provide *at least 3 test cases you devised on your own* (as separate text files under the `test/` directory). *Your A5 test cases can be the same ones as A1-A4, or you can add more. For simplicity assume up to 3 input files (no need to test with more).*

Submit the following files:

* `shell.c`
* `countnames.c`
* `README.md`
* `output.pdf`

Your `README.md` must include:

* student name(s)
* instructions on how to compile the programs
* instructions on how to run the test cases
* expected output for each test case
* explanation of what each test case is testing

At the end of the README include the following sections:

* Lessons learned
* References
* Acknowledgements

Also submit a pdf file "output.pdf" showing screenshots of your compilation and execution on all the testcases. The graders may compile the assignment code and test it, but if your output.pdf looks convincing it could save them some time. Remember, in zyLab you need to click "Submit" to submit your work.

---

## Compilation

Example compilation commands:

```bash
gcc -o shell shell.c -Wall -Werror
gcc -o countnames countnames.c -Wall -Werror
```

Your code should compile *without warnings*.

---

## Grading

| Category | Weight |
| :--- | :--- |
| overall requirements are satisfied, output is printed correctly | 20% |
| compiles cleanly (no errors/warnings) | 5% |
| no memory leaks detected with valgrind | 10% |
| malloc is used correctly | 10% |
| free is used correctly | 20% |
| dynamically sized array with realloc | 10% |
| correct aggregation of name counts | 15% |
| storing char * strings in array is implemented correctly (strdup) | 5% |
| code readability and comments | 3% |
| submission files and documentation | 2% |

---

## Notes about getline fgets strdup

1. For `getline` you should either set the ptr you pass in to NULL or malloc it and it will realloc if needed. In the end you need to free it just once.
2. Both `fgets` and `getline` return null-terminate the buffer of char array.
3. For `strdup` you should not malloc the pointer that will get returned.

## Some Hashing notes

For this assignment it is ok if as hash function to locate a slot, you simply use the Ascii value of the first char in a name. To reduce the chance of collisions, though, a better approach is to compute a [radix hashCode](https://www.google.com/search?client=safari&rls=en&q=radix+hashCode+function&ie=UTF-8&oe=UTF-8) for converting names to numbers.

* **Open addressing**: all elements are stored in the array (no linked list). When collision (an array slot is full) jump to the next candidate slot.
* **Open addressing with coalesced hashing**: When you have a collision, put the element in the first free slot anywhere in the array and point to that with the next pointer from the original slot.
