# Assignment 6: Multi-threaded countnames

## Objectives

* Learn about multi-threaded programming and locking shared variables.
* For this exercise you will re-use parts of your previous assignments.

## Background

In **Assignment 1**, you implemented the `countnames` utility that reads a file containing names and counts how many times each name appears.

In **Assignment 2**, you introduced **parallel processing**. The shell program spawned multiple child processes so that multiple files could be processed simultaneously.

In **Assignment 3**, child processes communicated their results back to the parent using **pipes**, so that the parent process could aggregate the results.

In **Assignment 4**, you replaced **pipe communication with shared memory using the `mmap()` system call**.

In **Assignment 5**, you implemented dynamic memory allocation in place of static sized memory allocations.

In this Assignment, you will implement multi-threading in place of child processes.

The command line interface is the same as in the other assignments. For example, running on the `names?.txt` files:

```bash
./countnames names1.txt names2.txt
```

Which will output to **stdout**:

```text
Tom Wu: 4
Jenn Xu: 2
```

## Tasks

### 1. Replace child processes with threads

Replace child processes (writing their individual namescounts to mmapped spaces) with threads. The threads will update the parent shell's malloc'ed space (from A5) directly, thus they don't need their own individual mmapped spaces.

### 2. Implement Mutex Locking

You will need to implement a lock on the parent's malloc'ed space to make the multi-thread updates atomic to ensure the data won't get corrupted.

### 3. Threaded countnames implementation

You will modify your `countnames` code to employ multi-threading (thread 1 is assigned the first file, thread 2 the second file, etc.), creating a new threaded `countnames`.

* The functionality remains the same: expect `argv-1` input files.
* Your A5 implementation is the starting point, but remove multi-processing parts (`fork`, `wait`, etc.).
* If a file does not exist, print a suitable error message and continue with other files.
* You will have `argv-1` threads updating the same name counts in parallel.
* Thread 1 reads from the first file and updates the shared data structure until EOF.
* Thread 2 does the same for the second file.
* **Critical Section**: You must protect the updating of name counts with mutex locks to ensure threads do not corrupt the data or read invalid values.

After all threads are done, the process will print the aggregated counts (same as previous assignments).

### Compilation

Compile your code with `pthread` as follows:

```bash
gcc -D_REENTRANT -pthread -o countnames countnames.c -Wall -Werror
```

## Submission

Besides the given test cases, you must provide **at least 3 test cases of your own** (separate text files under the `test/` directory).

* You can reuse A1-A5 test cases or add new ones.
* For simplicity, assume up to 3 input files.

### Files to Submit

* `shell.c`
* `countnames.c`
* `README.md`
* `output.pdf` (Screenshots of compilation and execution)

### README Requirements

Your `README.md` must include:

* Student name(s)
* Compilation instructions
* Instructions on how to run test cases
* Expected output for each test case
* Explanation of what each test case is testing

Include these sections at the end:

* \# Lessons learned
* \# References
* \# Acknowledgements

## Compilation & Execution Examples

```bash
gcc -o shell shell.c -Wall -Werror
gcc -o countnames countnames.c -Wall -Werror
```

Your code should compile **without warnings**.

## Grading

| Category | Weight |
| :--- | :--- |
| Overall requirements satisfied, output printed correctly | 20% |
| Compiles cleanly (no errors/warnings) | 5% |
| No memory leaks detected with Valgrind | 10% |
| Correctly uses pthread (create and join) | 15% |
| Correctly uses mutex locking on the name counts data structure | 15% |
| All threads run concurrently (in parallel), code is thread-safe | 15% |
| Updating the aggregated array of namecounts is correct | 15% |
| Code readability and comments | 3% |
| Submission files and documentation | 2% |

## Memory Leak Detection

You can use `valgrind` to ensure there are no memory leaks:

```bash
gcc -Wall -Werror -g example.c -o example
valgrind --leak-check=full ./example
```

## Additional Notes

### getline, fgets, strdup

1. **getline**: Set the pointer to `NULL` or `malloc` it (it will `realloc` if needed). Free it once at the end. [StackOverflow Reference](https://stackoverflow.com/questions/42478868/how-do-i-properly-free-memory-related-to-getline-function)
2. **fgets/getline**: Both return null-terminated character buffers. [StackOverflow Reference](https://stackoverflow.com/questions/1660228/does-fgets-always-terminate-the-char-buffer-with-0)

### Discord Chat Notes

* **Hashtable Location**: For A6, it's fine for the hashtable to be just in `countnames` since you're using locks for multi-threaded updates.
* **Shell Integration**: Submit `shell.c` for consistency, but you don't need to modify it. The grader will likely run `countnames` individually.
* **Output Handling**: Since the hashtable isn't passed back to the parent through memory, `countnames` can write to `pid.out` and the shell can `cat pid.out`.
* **Stdin Support**: If `argc == 1`, `countnames` should read from `stdin`.
* **Future Work**: Advanced techniques (like CUDA-style unified memory) for sharing memory between processes were considered for A7, but A7 will not be assigned.
