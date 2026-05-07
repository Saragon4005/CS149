# CS149 SP26 A6

Steve Rubin <steven.rubin@sjsu.edu>
Raven Keszei <raven.keszei@sjsu.edu>

## Compilation

To compile the program, use the following commands in the terminal:

```bash
gcc -D_REENTRANT -pthread -o countnames countnames.c namecount.c -Wall -Werror
gcc -D_REENTRANT -pthread -o shell shell.c namecount.c -Wall -Werror
```

**Use of Makefile to compile**
Alternatively, you can use the provided Makefile to compile the program:

```bash
make all
```

Artifacts and compiled binaries can be cleaned with:

```bash
make clean
```

## Test Cases and Execution

To run the program with test files, execute the shell and then provide the command with the input files you wish to test.

```bash
./shell
% ./countnames test/names1.txt test/names2.txt test/namesB.txt
```

Alternatively, you can run tests in a single batch using `echo` and piping it to the shell (or run `countnames` standalone):

```bash
echo "./countnames test/names1.txt test/names2.txt test/namesB.txt" | ./shell
# or directly
./countnames test/names1.txt test/names2.txt test/namesB.txt
```

### Expected Outputs and Edge Cases Tested (Devised Test Cases)

* **`test/names1.txt`, `test/names2.txt`**
  **Expected Output:**
  `Jenn Xu: 2`
  `Tom Wu: 4`
  **Explanation:** Tests parallel reads tracking identically named elements cross-files. Both files contain elements matching each other. Validates that the multi-threaded architecture can merge identical string hashes back into the parent's dynamically allocated hash table properly without race conditions.

* **`test/namesB.txt`**
  **Expected Output:**
  `Warning - test/namesB.txt line 2 is empty.`
  `Warning - test/namesB.txt line 5 is empty.`
  `Nicky: 1`
  `Yuan Cheng Chang: 3`
  `John Smith: 1`
  `Dave Joe: 2`
  **Explanation:** Tests internal duplication within a single document stream without external intersections, as well as handling of empty lines. The thread safely ignores the empty lines (logging warnings to `stderr`) and deduplicates names locally via the shared mutex-locked global hash table.

* **`test/names_white_space_trail.txt`**
  **Expected Output:**
  `Tom : 1`
  `Tom  : 1`
  `Tom   : 1`
  `Tom: 1`
  **Explanation:** Tests file processing when names look identical but have varying amounts of trailing whitespace. Ensures that the dynamic string allocations (`strdup` and `getline`) correctly capture the exact literal byte string of the names, treating differently-spaced identical words as unique entries in the hash map.

* **`test/names_long.txt`**
  **Expected Output:**
  `% [100+ lines of Name: Count pairs]`
  `%`
  **Explanation:** Tests file processing when the input dataset is large and contains many distinct names. Validates that the dynamically resizing hash table correctly scales (via `realloc`) multiple times without memory corruption or locks failing under heavier load.

Note: Exact output ordering may vary depending on the dynamic hash table traversal, but total aggregate counts will match exactly.

## Threading Mechanism and Memory Management

For concurrent processing, our implementation utilizes **pthreads** for file reading, and a **Dynamically Allocated Hash Table** protected by a **Mutex Lock** for thread-safe aggregation.

1. **Hash Table Initialization:** The main thread initializes a dynamic hash table (`NameCountData **hashTable`) using `malloc` with an initial size of 10. A single global `pthread_mutex_t` (`hashLock`) is instantiated.
2. **Thread Spawning:** `countnames` (or `shell`) iterates over the input arguments. For each filename provided, it calls `pthread_create` to launch a concurrent `countNamesThread` worker.
3. **Concurrent Processing:** Each thread uses `getline` to dynamically read lines from its respective file.
4. **Mutex Locking (Critical Section):** When a thread parses a valid name, it calls `insertName`. This function immediately acquires `hashLock`. It searches the hash table and either increments an existing count or dynamically duplicates the string (`strdup`) to create a new node.
5. **Dynamic Aggregation:** If the hash table's load factor exceeds 2 while locked, the thread performing the insert dynamically expands the table size using `realloc` and rehashes all entries before releasing the lock.
6. **Clean Up:** After all threads are joined via `pthread_join`, the main thread explicitly frees all strings, structs, and arrays using `freeTable()` to ensure zero memory leaks.

## Lessons Learned

* Designing thread-safe data structures requires careful placement of mutex locks to ensure entire read-modify-write operations (like traversing a linked list and adding a node) are atomic.
* Moving from multi-processing to multi-threading significantly simplifies inter-process communication (IPC) since threads inherently share the same heap memory, eliminating the need for `mmap` or pipes.
* Memory management becomes more nuanced in a multi-threaded environment; ensuring that `malloc`, `realloc`, and `free` operations don't interleave dangerously requires robust locking mechanisms, especially during rehashing.

## References

* `man pthread_create` and `man pthread_join` specifications
* `man pthread_mutex_lock` standard POSIX specifications
* `man realloc` and dynamic memory management

## Acknowledgements

* Used AI to audit the codebase and verify that the multi-threaded architecture strictly adhered to the assignment instructions.
