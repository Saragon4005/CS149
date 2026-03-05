**CS149 SP26 A1**
3/4/2026
Steve Rubin <steven.rubin@sjsu.edu>
Raven Keszei <raven.keszei@sjsu.edu>


# compilation
To compile the program, use the following command in the terminal:

```
gcc -o countnames countnames.c -Wall -Werror
gcc -o shell1 shell1.c -Wall -Werror
```

# use of Makefile to compile
Alternatively, you can use the provided Makefile to compile the program.
```
make all
```

Artifacts can be cleaned with
```
make clean
```

# test cases
To run the program with a file input, use the following command:
```
./shell1
```
Input should be ./countnames test/test1.txt test/test2.txt ...

# Test Execution Using shell1

This section demonstrates running all test cases in a single batch using shell1, which executes countnames for multiple test files and creates .out and .err files for each.

## Running All Tests at Once

**Preparation:**
```bash
make clean
make all
```

**Running all tests via shell1:**
```bash
echo "./countnames test/empty.txt test/names.txt test/names1.txt test/names2.txt test/names_white_space_prefix.txt test/names_white_space_trail.txt test/names_long_redundant.txt test/names_long_redundant1.txt test/names_long_redundant2.txt test/names_long_redundant3.txt test/names_long.txt" | ./shell1
```

**Verification of created files:**
```bash
ls -la *.out *.err
```

**Results:**
All 11 test files successfully created their corresponding .out and .err files (22 files total):

| PID   | Test File                      | .out Size | .err Size | Notes                                    |
|-------|--------------------------------|-----------|-----------|------------------------------------------|
| 23570 | empty.txt                      | 0 bytes   | 0 bytes   | Empty file, no output                    |
| 23571 | names.txt                      | 55 bytes  | 54 bytes  | Multiple names with empty line warnings  |
| 23572 | names1.txt                     | 10 bytes  | 27 bytes  | Duplicate names with empty line warning  |
| 23573 | names2.txt                     | 21 bytes  | 0 bytes   | Basic duplicate counting                 |
| 23574 | names_white_space_prefix.txt   | 57 bytes  | 0 bytes   | Leading whitespace test                  |
| 23575 | names_white_space_trail.txt    | 34 bytes  | 0 bytes   | Trailing whitespace test                 |
| 23576 | names_long_redundant.txt       | 1548 bytes| 28 bytes  | Many names with some duplicates          |
| 23577 | names_long_redundant1.txt      | 678 bytes | 108 bytes | Multiple consecutive empty lines         |
| 23578 | names_long_redundant2.txt      | 749 bytes | 0 bytes   | Many unique names                        |
| 23579 | names_long_redundant3.txt      | 706 bytes | 0 bytes   | Names with some appearing twice          |
| 23580 | names_long.txt                 | 1711 bytes| 0 bytes   | Large file with 99 single-space "names"  |

This demonstrates that shell1 successfully executes countnames for all test files in sequence, with each execution producing separate .out and .err files identified by their process IDs.


# Lessons Learned
 * Ensure free()'ing of any allocated memory
 * Watch out for infite loops

# References
 * C Programming Language, 2nd Edition, by Brian W. Kernighan and Dennis M. Ritchie.
  
# Acknowledgements
 * Used Claude Code to automate testing of the program across the entire test suite and verify outputs against expected results and to clean up README.md formatting.
 