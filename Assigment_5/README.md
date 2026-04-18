# CS149 SP26 A5

Steve Rubin <steven.rubin@sjsu.edu>
Raven Keszei <raven.keszei@sjsu.edu>

## Compilation

To compile the program, use the following commands in the terminal:

```bash
gcc -o countnames countnames.c -Wall -Werror
gcc -o shell shell.c -Wall -Werror
```

**Use of Makefile to compile**
Alternatively, you can use the provided Makefile to compile the program:

```bash
make all
```

Artifacts and intermediate map logs can be cleaned with:

```bash
make clean
```

## Test Cases and Execution

To run the program with test files, execute the shell and then provide the command with the input files you wish to test.

```bash
./shell
% ./countnames test/names1.txt test/names2.txt test/namesB.txt
```

Alternatively, you can run tests in a single batch using `echo` and piping it to the shell:

```bash
echo "./countnames test/names1.txt test/names2.txt test/namesB.txt" | ./shell
```

### Expected Outputs and Edge Cases Tested (Devised Test Cases)

* **`test/names1.txt`, `test/names2.txt`**
  **Expected Output:**
  `% Jenn Xu: 2`
  `Tom Wu: 4`
  `%`
  **Explanation:** Tests parallel reads tracking identically named elements cross-files. Both files contain elements matching each other. Validates that the parallel architecture can merge identical string hashes back into the parent's dynamically allocated hash table properly.

* **`test/namesB.txt`**
  **Expected Output:**
  `% Nicky: 1`
  `Dave Joe: 2`
  `Yuan Cheng Chang: 3`
  `John Smith: 1`
  `%`
  **Explanation:** Tests internal duplication within a single document stream without external intersections. It forces the singleton binary payload to deduplicate locally before passing it to the global hash table map.

* **`test/names_long.txt`**
  **Expected Output:**
  `% Name: Count`
  `%`
  **Explanation:** Tests file processing when individual lines exceed standard character boundaries. Warnings are safely logged via `stderr` outputs redirected dynamically to `<child process_id>.err` files. This ensures standard error handling flows are not scrambled under concurrent conditions. The dynamic string allocations (`strdup` and `getline`) correctly capture names of any length.

Note: Exact output ordering may vary depending on the dynamic hash table traversal, but total aggregate counts will match exactly.

## IPC Mechanism and Memory Management

For communication between the child processes (`countnames`) and the parent process (`shell`), our implementation utilizes **Shared Memory Using Mmap** for child outputs, and a **Dynamically Allocated Hash Table** for parent aggregation.

1. **Hash Table Initialization:** The `shell` parent initializes a dynamic hash table (`NameCountData **hashTable`) using `malloc` with an initial size of 10.
2. **Child Processing:** Upon forking, `shell` calls `countnames`, passing individual filename targets.
3. **Map Outs:** As a child runs, it captures name strings into an intermediate local map bound to `{PID}.out`. The strings in the child are managed dynamically using `getline` and `strdup`.
4. **Memory Injection:** Once the child exit signals `wait()`, the `shell` reads the child's raw binary object file `{PID}.out` via `mmap`.
5. **Dynamic Aggregation:** The parent iterates over the child's records, duplicating the string (`strdup`) and aggregating the counts into the hash table. If the hash table's load factor exceeds 2, it dynamically expands the table size using `realloc` and rehashes the entries.
6. **Clean Up:** After results are printed, the parent explicitly frees all strings, structs, and arrays to ensure zero memory leaks.

## Lessons Learned

* Managing memory explicitly using `malloc`, `realloc`, and `free` requires strict tracking of object lifetimes to prevent leaks, especially when dealing with duplicated strings.
* Implementing a dynamically resizing hash table provides an efficient O(1) average lookup time for large datasets, vastly outperforming locking mechanisms on linear arrays.
* The transition from static memory mapping to dynamic heap allocation allows the system to easily scale beyond arbitrary array limits.

## References

* `man mmap` standard POSIX specifications
* `man realloc` and dynamic memory management

## Acknowledgements

* Used Gemini to audit the memory management of our `NameCountData` type, catching file descriptor leaks and unhandled `asprintf` responses.
