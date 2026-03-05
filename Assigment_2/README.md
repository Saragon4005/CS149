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

## Test Case 0: nonexistent file
```./countnames test/nonexistent.txt
```
Output:
```error: cannot open file
```
Purpose: This test case checks that the program correctly handles the case where the specified file does not exist

## Test Case 1: names_white_space_prefix.txt
```
./countnames test/names_white_space_prefix.txt
```
Output:
```
     Tom: 1
    Tom: 1
   Tom: 1
  Tom: 1
 Tom: 1
Tom: 1
```
Purpose: This test case checks that the program handles leading white spaces correctly and counts them as different names.

## Test Case 2: names_white_space_trail.txt
```
./countnames test/names_white_space_trail.txt
```
Output:
```
Tom   : 1
Tom  : 1
Tom : 1
Tom: 1
```
Purpose: This test case checks that the program handles trailing white spaces correctly and counts them as different names.

## Test Case 3: empty.txt
```
./countnames test/empty.txt
```
Output: (none)

Purpose: This test case checks that the program correctly handles an empty file (0 length) and does not produce any output.

## Test Case 4: names.txt
```
./countnames test/names.txt
```
Output:
```
Warning - Line 2 is empty.
Warning - Line 5 is empty.
John Smith: 1
Yuan Cheng Chang: 3
Dave Joe: 2
Nicky: 1
```
Purpose: This test case checks basic functionality with multiple names and some duplicates, while also testing handling of empty lines.

## Test Case 5: names1.txt
```
./countnames test/names1.txt
```
Output:
```
Warning - Line 3 is empty.
Tom Wu: 3
```
Purpose: This test case checks duplicate name handling and empty line detection.

## Test Case 6: names2.txt
```
./countnames test/names2.txt
```
Output:
```
Tom Wu: 1
Jenn Xu: 2
```
Purpose: This test case checks basic duplicate counting with multiple names.

## Test Case 8: names_long_redundant.txt
```
./countnames test/names_long_redundant.txt
```
Output:
```
Warning - Line 97 is empty.
Brandon Brown: 1
Sofia Thompson: 1
Nicholas Davis: 1
Hannah Garcia: 1
Tyler Martinez: 1
Elizabeth Adams: 1
Jacob Wright: 1
Mia Lee: 1
Daniel Anderson: 1
Samantha Martinez: 1
Olivia Lee: 1
Christopher Lee: 1
Emily Rodriguez: 1
Alexander Clark: 1
Makayla Hernandez: 1
Brandon Davis: 1
Isabella Adams: 1
Natalie Martin: 1
David Wright: 1
Ava Smith: 1
William Scott: 1
Chloe Kim: 1
Ella Garcia: 1
Benjamin Davis: 1
Sophia Turner: 1
Ryan Rodriguez: 1
Aria Wright: 1
Lucas Davis: 1
Madison Martinez: 1
Ethan Brown: 1
Emma Hernandez: 1
Avery Lee: 1
Nicholas Jackson: 1
Leah Lee: 1
Jayden Robinson: 1
Daniel Lee: 1
Abigail Perez: 1
Christopher Anderson: 1
Aaliyah Brown: 1
Luke Davis: 1
Makenna Wright: 1
James Lewis: 1
Lauren Kim: 1
Dylan Anderson: 1
Gabriella Clark: 1
Avery Adams: 2
Samuel Davis: 1
Lily Johnson: 1
Jonathan Lee: 2
Ella Wilson: 1
William Turner: 1
Ava Jones: 1
Christian Martinez: 1
Grace Lee: 1
Brandon Smith: 1
Sofia Hernandez: 1
Nicholas Nguyen: 1
Makayla Turner: 1
Alexander Rodriguez: 1
Hannah Scott: 1
Tyler Thompson: 1
Elizabeth Martinez: 1
Jacob Cooper: 1
Aaliyah Garcia: 1
Benjamin Wright: 1
Samantha Perez: 1
Ryan Lee: 1
Chloe Baker: 1
Ethan Martin: 1
Natalie Adams: 1
Nathan Davis: 2
Isabella Clark: 1
Joshua Wright: 1
Madison Green: 1
William Anderson: 1
Victoria Wilson: 1
Matthew Davis: 1
Aria Perez: 1
Andrew Lee: 2
Avery Taylor: 2
Christopher Gonzalez: 1
Evelyn Jackson: 1
Anthony Martinez: 1
Olivia Brown: 1
David Kim: 1
Mia Hernandez: 1
Daniel Rodriguez: 1
Sophia Lee: 1
Michael Johnson: 1
Emily Davis: 1
John Smith: 1
```
Purpose: This test case checks handling of many names with some duplicates and empty lines in the middle of the file.

## Test Case 9: names_long_redundant1.txt
```
./countnames test/names_long_redundant1.txt
```
Output:
```
Warning - Line 2 is empty.
Warning - Line 4 is empty.
Warning - Line 6 is empty.
Warning - Line 8 is empty.
AMANDA CARTER: 1
DEBRA NELSON: 1
MARTHA GONZALEZ: 1
PAMELA BAKER: 1
KATHLEEN ADAMS: 1
VIRGINIA GREEN: 1
REBECCA SCOTT: 1
ANNA HILL: 1
AMY LOPEZ: 1
BRENDA WRIGHT: 1
MELISSA KING: 1
ANGELA HERNANDEZ: 1
CYNTHIA YOUNG: 1
SHIRLEY ALLEN: 1
JESSICA HALL: 1
DEBORAH WALKER: 1
KIMBERLY LEE: 1
SARAH LEWIS: 1
LAURA RODRIGUEZ: 1
MICHELLE CLARK: 1
SHARON ROBINSON: 1
RUTH MARTINEZ: 1
CAROL GARCIA: 1
DONNA THOMPSON: 1
SANDRA MARTIN: 1
HELEN HARRIS: 1
BETTY WHITE: 1
KAREN JACKSON: 1
NANCY THOMAS: 1
LISA ANDERSON: 1
DOROTHY TAYLOR: 1
MARGARET MOORE: 1
SUSAN WILSON: 1
MARIA MILLER: 1
JENNIFER DAVIS: 1
ELIZABETH BROWN: 1
BARBARA JONES: 1
LINDA WILLIAMS: 1
PATRICIA JOHNSON: 1
MARY SMITH: 1
```
Purpose: This test case checks handling of multiple consecutive empty lines interspersed with names.

## Test Case 10: names_long_redundant2.txt
```
./countnames test/names_long_redundant2.txt
```
Output:
```
SARA A PERRY: 1
LOUISE B JENKINS: 1
KATHRYN COLEMAN: 1
ANDREA HENDERSON: 1
MARILYN ROSS: 1
RACHEL BARNES: 1
LORI WOOD: 1
JANE BENNETT: 1
IRENE PRICE: 1
TAMMY SANDERS: 1
DENISE KELLY: 1
BEVERLY BROOKS: 1
THERESA WATSON: 1
KATHY JAMES: 1
CHRISTINA RAMIREZ: 1
JUDY GRAY: 1
NICOLE PETERSON: 1
KELLY TORRES: 1
JANICE WARD: 1
ROSE HOWARD: 1
JUDITH COX: 1
ASHLEY RICHARDSON: 1
JOAN COOPER: 1
KATHERINE RIVERA: 1
MILDRED BAILEY: 1
CHERYL MURPHY: 1
JEAN BELL: 1
EVELYN MORGAN: 1
GLORIA COOK: 1
DORIS REED: 1
TERESA ROGERS: 1
HEATHER MORRIS: 1
JULIE SANCHEZ: 1
ALICE STEWART: 1
DIANE COLLINS: 1
JOYCE EDWARDS: 1
ANN EVANS: 1
FRANCES PARKER: 1
CATHERINE CAMPBELL: 1
JANET PHILLIPS: 1
MARIE TURNER: 1
CHRISTINE ROBERTS: 1
CAROLYN PEREZ: 1
STEPHANIE MITCHELL: 1
```
Purpose: This test case checks handling of many unique names (all count 1).

## Test Case 11: names_long_redundant3.txt
```
./countnames test/names_long_redundant3.txt
```
Output:
```
DEBORAH WALKER: 1
KIMBERLY LEE: 1
SARAH LEWIS: 1
LAURA RODRIGUEZ: 1
MICHELLE CLARK: 1
SHARON ROBINSON: 1
RUTH MARTINEZ: 1
CAROL GARCIA: 1
DONNA THOMPSON: 1
SANDRA MARTIN: 1
HELEN HARRIS: 1
BETTY WHITE: 1
KAREN JACKSON: 1
NANCY THOMAS: 1
LISA ANDERSON: 1
DOROTHY TAYLOR: 1
MARGARET MOORE: 1
SUSAN WILSON: 1
MARIA MILLER: 1
JENNIFER DAVIS: 1
ELIZABETH BROWN: 2
BARBARA JONES: 2
LINDA WILLIAMS: 2
PATRICIA JOHNSON: 2
MARY SMITH: 2
ROBIN HAYES: 1
EMILY DIAZ: 1
LILLIAN GRIFFIN: 1
ANNIE RUSSELL: 1
DIANA ALEXANDER: 1
PAULA BRYANT: 1
NORMA GONZALES: 1
PHYLLIS FOSTER: 1
TINA SIMMONS: 1
LOIS BUTLER: 1
RUBY WASHINGTON: 1
JULIA FLORES: 1
BONNIE HUGHES: 1
WANDA M PATTERSON: 1
JACQUELINE K LONG: 1
ANNE J POWELL: 1
```
Purpose: This test case checks duplicate counting with some names appearing exactly twice.

## Test Case 12: names_long.txt
```
./countnames test/names_long.txt
```
Output:
```
ROBIN HAYES: 1
EMILY DIAZ: 1
LILLIAN GRIFFIN: 1
ANNIE RUSSELL: 1
DIANA ALEXANDER: 1
PAULA BRYANT: 1
NORMA GONZALES: 1
PHYLLIS FOSTER: 1
TINA SIMMONS: 1
LOIS BUTLER: 1
RUBY WASHINGTON: 1
JULIA FLORES: 1
BONNIE HUGHES: 1
WANDA M PATTERSON: 1
JACQUELINE K LONG: 1
ANNE J POWELL: 1
SARA A PERRY: 1
LOUISE B JENKINS: 1
KATHRYN COLEMAN: 1
ANDREA HENDERSON: 1
MARILYN ROSS: 1
RACHEL BARNES: 1
LORI WOOD: 1
JANE BENNETT: 1
IRENE PRICE: 1
TAMMY SANDERS: 1
DENISE KELLY: 1
BEVERLY BROOKS: 1
THERESA WATSON: 1
KATHY JAMES: 1
CHRISTINA RAMIREZ: 1
JUDY GRAY: 1
NICOLE PETERSON: 1
KELLY TORRES: 1
JANICE WARD: 1
ROSE HOWARD: 1
JUDITH COX: 1
ASHLEY RICHARDSON: 1
JOAN COOPER: 1
KATHERINE RIVERA: 1
MILDRED BAILEY: 1
CHERYL MURPHY: 1
JEAN BELL: 1
EVELYN MORGAN: 1
GLORIA COOK: 1
DORIS REED: 1
TERESA ROGERS: 1
HEATHER MORRIS: 1
JULIE SANCHEZ: 1
ALICE STEWART: 1
DIANE COLLINS: 1
JOYCE EDWARDS: 1
ANN EVANS: 1
FRANCES PARKER: 1
CATHERINE CAMPBELL: 1
JANET PHILLIPS: 1
MARIE TURNER: 1
CHRISTINE ROBERTS: 1
CAROLYN PEREZ: 1
STEPHANIE MITCHELL: 1
AMANDA CARTER: 1
DEBRA NELSON: 1
MARTHA GONZALEZ: 1
PAMELA BAKER: 1
KATHLEEN ADAMS: 1
VIRGINIA GREEN: 1
REBECCA SCOTT: 1
ANNA HILL: 1
AMY LOPEZ: 1
BRENDA WRIGHT: 1
MELISSA KING: 1
ANGELA HERNANDEZ: 1
CYNTHIA YOUNG: 1
SHIRLEY ALLEN: 1
JESSICA HALL: 1
DEBORAH WALKER: 1
KIMBERLY LEE: 1
SARAH LEWIS: 1
LAURA RODRIGUEZ: 1
MICHELLE CLARK: 1
SHARON ROBINSON: 1
RUTH MARTINEZ: 1
CAROL GARCIA: 1
DONNA THOMPSON: 1
SANDRA MARTIN: 1
HELEN HARRIS: 1
BETTY WHITE: 1
KAREN JACKSON: 1
NANCY THOMAS: 1
LISA ANDERSON: 1
DOROTHY TAYLOR: 1
MARGARET MOORE: 1
SUSAN WILSON: 1
MARIA MILLER: 1
JENNIFER DAVIS: 1
ELIZABETH BROWN: 1
BARBARA JONES: 1
LINDA WILLIAMS: 1
PATRICIA JOHNSON: 1
 : 99
MARY SMITH: 1
```
Purpose: This is test case checks a large number of empty lines, that only contain a space.  This tests that the program correctly counts the empty line with a space as a name (a line with just a single space is a name, a line with two spaces is another name, etc).)

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
 