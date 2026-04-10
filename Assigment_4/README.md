# CS149 SP26 A4

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

To run the program with test files, execute the shell and then provide the command with up to `MAX_CHILDREN` (64) input files.

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
  **Explanation:** Tests parallel reads tracking identically named elements cross-files. Both files contain elements matching each other. Validates that the parallel architecture can merge identical string hashes across separate memory map spaces back into the summation integer fields properly.

* **`test/namesB.txt`**
  **Expected Output:**
  `% Nicky: 1`
  `Dave Joe: 2`
  `Yuan Cheng Chang: 3`
  `John Smith: 1`
  `%`
  **Explanation:** Tests internal duplication within a single document stream without external intersections. It forces the singleton binary payload to deduplicate locally before passing it to the global shared map struct.

* **`test/names_long.txt`**
  **Expected Output:**
  `% Name: Count`
  `%`
  **Explanation:** Tests file processing when individual lines exceed the 30-character boundary struct ceiling. Warnings are safely logged via `stderr` outputs redirected dynamically to `<child process_id>.err` files. This ensures standard error handling flows are not scrambled under concurrent conditions.

Note: Exact output ordering may vary depending on parsing, but total aggregate counts will match exactly.

## IPC Mechanism

For communication between the child processes (`countnames`) and the parent process (`shell`), our implementation utilizes **Shared Memory Using Mmap** instead of OS kernel pipes.

1. **Global Aggregation Footprint:** The `shell` parent initializes a large cross-process shared `mmap` allocation (`GLOBAL`), sized systematically to `(MAX_CHILDREN + 1) * regionSize`. The `+ 1` accounts for a reserved final "summation region" section.
2. **Child Processing:** Upon forking, `shell` calls `countnames`, passing individual filename targets.
3. **Map Outs:** As a child runs, it captures name strings into an intermediate local map bound to `{PID}.out`.
4. **Memory Injection:** Once the child exit signals `wait()`, the `shell` reads the child's raw binary object file `{PID}.out` strictly into the isolated geometric boundaries carved for its exact sub-index within `GLOBAL`.
5. **Summation & Zero-Termination:** After all subsets resolve into the `GLOBAL` canvas, the values iterate down tracking the `0-length record` sentinels, accumulating counts into the summation block and flushing it to the standard output terminal.

## Lessons Learned

* Inter-process synchronization utilizing shared memory forces meticulous boundary definitions compared to dynamic length POSIX pipe chunks. Pre-calculating memory allocations using discrete structs simplifies iterating large raw arrays safely over void pointers.
* Reusing standard library string operations like `strncpy` to buffer precisely sized characters inside `void*` blocks runs the serious risk of silently erasing `\0` terminator bits if the characters consume the entire allowed slot width. Leaving `MAX_NAME_LENGTH + 1` mathematically eliminates runover bugs when fetching `printf("%s")` arrays.
* When spawning file-mapped memory interfaces to connect to child outputs, cleaning dynamically generated artifacts post-execution is a critical workflow to prevent disk cluster pollution. Including dynamic wildcard sweeps under a standard `Makefile` `clean` routine effectively solves this.

## References

* `man mmap` standard POSIX specifications

## Acknowledgements

* Used Gemini to audit the buffer sizes of our `NameCountData` type, catching a 30-character exact-match overflow.
