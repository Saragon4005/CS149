# CS149 SP26 A3

3/25/2026
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

Artifacts can be cleaned with:

```bash
make clean
```

## Test Cases and Execution

To run the program with test files, execute the shell and then provide the command with up to 3 input files.

```bash
./shell
% ./countnames test/names1.txt test/names2.txt test/namesB.txt
```

Alternatively, you can run tests in a single batch using `echo` and piping it to the shell:

```bash
echo "./countnames test/names1.txt test/names2.txt test/namesB.txt" | ./shell
```

### Expected Outputs and Edge Cases Tested

* **`test/names1.txt`, `test/names2.txt`**
  Expected Output: `% Jenn Xu: 2`, `Tom Wu: 4`, `%`
  Tests aggregation of identical names across multiple files ("Tom Wu" appears 3x in names1 and 1x in names2, correctly aggregating to 4). Also tests empty line warnings printed to `.err` files.

* **`test/namesB.txt`**
  Expected Output: `% Nicky: 1`, `Dave Joe: 2`, `Yuan Cheng Chang: 3`, `John Smith: 1`, `%`
  Tests accurate singular extraction and internal duplicate collapsing within a single file with diverse format blocks.

* **`test/names_long.txt`**
  Expected Output: `% Name: Count`, `%`
  Tests file processing when individual lines exceed the 30-character boundary. Warnings are safely logged to `.err`.

* **`test/names1.txt`, `test/names2.txt`, `test/names1.txt`, `test/names_long.txt`**
  Expected Output: `% PATRICIA JOHNSON: 1`, `(98 other names)`, `Jenn Xu: 2`, `Tom Wu: 7`, `%`
  Tests 4+ concurrent tasks using duplicate and heavy-load files simultaneously. Validates robustness under multiple parallel IPC writes.

Note: Exact output ordering may vary depending on linked list insertion patterns, but total counts will match exactly.

### IPC Mechanism

For communication between the child processes (`countnames`) and the parent process (`shell`), our implementation utilizes **Option 1: pipes**. The shell process establishes a `pipe(pipefd)` for each child task to send its parsed `NameCountData` binary payloads. The child processes replace their standard output with the pipe's write end using `dup2(pipefd[1], STDOUT_FILENO)`.

## Lessons Learned

* When replacing binary data structures across interprocess pipes, using exactly matched struct definitions avoids alignment issues. However, if using string comparisons (e.g. `strcmp`) on strictly fixed-sized character arrays, missing null terminators can produce undefined reading boundaries.
* Inherited file descriptors in consecutive loops can spawn unintentional file descriptor leaks over heavily populated IPC chains if the previously opened read ends are not closed in subsequent forks.

## References

* C Programming Language, 2nd Edition, by Brian W. Kernighan and Dennis M. Ritchie.
* POSIX manual documentation for IPC pipelines (`pipe`, `dup2`).

## Acknowledgements

* Used Gemini to audit and identify undefined string comparison behaviors along with pipe descriptor leakages and verify all parts of the assigment are met.
