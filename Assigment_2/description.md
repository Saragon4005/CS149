# Assignment 2

## Objectives

* Understand how processes work and how files get processed in parallel  
* Understand how processes get executed in parallel  
* Understand how multi-processing can speed-up execution  
* Understand that processes run independently but they always have a parent process  
* Learn to program processes in such a way that a slow process won't cause the faster processes to stall; eliminate bottlenecks.  
* Write files and duplicate file descriptors.  
* Exit codes, signals.

## Background

It is commonplace to say that we are in a data-intensive era. Running processes in parallel is an essential capability of data-intensive computing. If you have spent a load of money on purchasing a 256 CPU (processor) computer you want to utilize it by running as many things as you can in parallel, rather than sequentially. Using all your 256 processors, you may see a 250-fold increase in performance (there is also some overhead when running things in parallel).

Your assignment \#2 implementation is actually taking advantage of your computer's parallelism capabilities, since the OS can schedule independent jobs to run in parallel on any CPU-processor.

You can think of this assignment as developing a new (but different) version of the GNU parallel program, except your command-line interface will be simpler and with a few extra functionalities that are missing, such as writing to log files.  [https://www.gnu.org/software/parallel](https://www.gnu.org/software/parallel)

## Tasks

For this assignment you will extend [shell1.c](shell1.c) (you can remove all the apue dependencies, so as to strip it down to a minimalistic version that doesn't need the apue files) and your **countnames.c** (from A1).

When you run shell1.c it offers a prompt-based shell, just like the shell you normally use on a UNIX-based system. When you run countnames on the shell with multiple arguments like this:

`% ./countnames names1.txt names2.txt names2.txt names3.txt....`

The shell1 process will spawn several child processes (one per input file) to compute the name counts of each file. Every countnames execution on one file will be a separate child process of the shell process. The ith child process of shell1 will process the ith file given. This differs from the typical bash shell where a command line spawns one child process; for A2 such a command line will spawn multiple child processes (with the shell1 process as their parent).

In other words, the shell process will spawn/fork multiple processes of countnames, running in a parallel fashion (each on one input file) and **wait for them to** complete. Each countnames process will read one of the input names files and count the names in it, and then output all the results to a file PID.out (replace PID with the process id).

You do not need to modify countnames.c much, other than writing to the PID.out file rather than the stdout (as you did in A1). In other words, the parent (the shell) will spawn all of the countnames children, each of which will print its name counts (for its input file) to PID.out, rather than stdout. Also same as A1, in case of no filenames given as command-line arguments (argc \== 1\) countnames.c still takes the input names from stdin.

Assume that you have several input files that have names in them separated by newlines (same as A1). You want to read in and count the names from each file. Same as in A1, assume all the files contain up to 100 different names with up to 30 characters in each. Some sample code for using fork/wait/exec/pipe, is provided under folders [code/proc](https://sjsu.instructure.com/courses/1619829/files/folder/code/proc) [code/ipc](https://sjsu.instructure.com/courses/1619829/files/folder/code/ipc) [code/a2\_a3](https://sjsu.instructure.com/courses/1619829/files/folder/code/a2_a3). The exec system call (or one of its variants) is going to be used by your code. It is recommended to use **execvp**, as it takes a flexible number of parameters. Some sample code for using exec is provided under folder [code/a2\_a3](https://sjsu.instructure.com/courses/1619829/files/folder/code/a2_a3).

If a file appears multiple times on the command line, the file will be processed multiple times (by separate child processes of the shell).

Your shell process will use **fork()** to spawn a countnames process for each of the files, such that we can compute name counts for each file in parallel. Your shell process will wait for all the child processes to finish using **wait()** (see [slides](https://docs.google.com/presentation/d/1tFAJHE88J3ylpWa9CZ5dhcAnSy1giA-YiUEUEIJej0Q/edit?slide=id.g27d9461c96e_1_127#slide=id.g27d9461c96e_1_127)

[Links to an external site.](https://docs.google.com/presentation/d/1tFAJHE88J3ylpWa9CZ5dhcAnSy1giA-YiUEUEIJej0Q/edit?slide=id.g27d9461c96e_1_127#slide=id.g27d9461c96e_1_127)

).

You must be able to handle files with any number of lines (including empty ones). You can assume that the files contain only valid ASCII characters separated by newlines (and possibly an occasional empty line, same as in A1). A name can include spaces, for example "Tom Wu" is one name.

For example, running on the [**`names1.txt`](test/names1.txt)**
`and [**`names2.txt`](test/names2.txt)**`

files:

`$ ./countnames names1.txt names2.txt`

will output name counts for each to **two** **files named PID.out (PID is a child process's pid)**. While:

`$ ./countnames names1.txt names2.txt names2.txt`

will output the name counts for each file to **three** **files named PID.out.**

If you wanted to combine the name counts from all PID.out files, you could use awk to combine name counts from many files, as follows (though you don't need to do this for this assignment): `awk '{ count[$2] += $1 } END { for (item in count) print count[item], item }' file1.out file2.out | sort -nr`

In the example above, if an input file contained empty lines you should also print to **PID.err** messages like (same as assignment 1, such empty lines in a file don't count and can be ignored for counting purposes):

**`Warning - file names1.txt line 3 is empty.`**

For example, process pid \#156 will write to 156.out and 156.err.

An objective of this assignment is to learn to program multiple processes in such a way that a slow process won't cause the faster processes to stall (eliminate bottlenecks). For example, assume the first file involves lots of lines (slow), while the second file contains only one line. The *wrong* way to implement the code would be for your second process to have to wait for the first process (which does more processing) to completely run through completion before the second process even starts. That would resemble the situation where a line-up of cars are trailing a slow-speed tractor on a single-lane road. For this reason, the parent should fork processes that will run independently and wait for any process to finish; the OS will handle the context switching between processes. See the slides linked above for the code to use.

Submission (same as A1)

Besides the given test cases, you must also provide at least 3 test cases you devised on your own (as separate text files you upload under the A2 test/ subdirectory).  
Submit a README.md file, showing at the top your student name(s), and showing exactly how to:  

* compile your code  
* run your compiled executable on each test case  
* show what output is expected for each test case  
* mention for each test case what situation/edge case it is testing for (all cases should test for something).

Additionally, at the bottom of the README.md file include these 3 sections (same as A1):

* Lessons learned : describe what you learned from this assignment (anything at all).  
* References : include citations (with links) to any source of information you found and used. For example, you could cite/link a forum discussion (stackoverflow), or the course Canvas/slides, or a website, or a book. In case you used a chatbot, you should make the chat public and link it.  
* Acknowledgements : mention any help you got from someone/thing. In case noone helped you directly, you may mention the course embedded tutor, specific people from the discord channel, class time, google, or an AI tool.

Also submit a pdf file "output.pdf" showing screenshots of your compilation and execution on all the testcases. The graders may compile the assignment code and test it, but if your output.pdf looks convincing it could save them some time.  Remember, in zyLab you need to click "Submit" to submit your work.

## Grading

| Category | Points |
| :---- | :---- |
| overall requirements of assignment are satisfied | 10% |
| compiles cleanly (no errors/warnings, use **gcc \-o countnames countnames.c \-Wall \-Werror; gcc \-o shell shell.c \-Wall \-Werror**) | 5% |
| output correctly for given test cases with input files under the A2/test/ subdirectory | 30% |
| correctly uses fork() | 10% |
| correctly uses wait() (or a variant of wait) | 10% |
| correctly uses exec() or execvp() | 10% |
| program flow is understandable | 5% |
| code is commented and indented (use of white space), same as in A1 | 10% |
| number of names (lines in input files, not unique names) that can be processed is not limited | 5% |
| submit **shell.c** and **countnames.c** with a README.md and output.pdf | 5% |

## submission

The submission contains **shell.c** and **countnames.c** and it doesn't need any other files to compile (unless you also put code in other files). The testcase files should be under the test/ subdirectory. Show in README.md how to run the test cases and expected output.  

## further details

The exit codes are the same as for A1.

* In all non-error cases, **countnames** should exit with status code 0, usually by returning a 0 from **main()** (or by calling **exit(0)**). This is called the exit code of a program and is different from what your program prints to stdout/stderr.  
* If *no file* or other parameters are specified on the command line, or an empty file with no numbers is given, **countnames** should just exit and return 0\.
* If a child process tries to **fopen()** a file and fails, it should print to stderr the message "error: cannot open file filename.txt" (followed by a newline) and exit with status code 1\. The parent can still process results for other children that succeeded.  
* Sample code to parse error code and signal of a completed process is in the slides\! See:  [https://docs.google.com/presentation/d/1tFAJHE88J3ylpWa9CZ5dhcAnSy1giA-YiUEUEIJej0Q/edit?folder=1HuyG3ez13YNEHILs7ky31jL4dIPhU4Sp\#slide=id.g27d9461c96e\_1\_127](https://docs.google.com/presentation/d/1tFAJHE88J3ylpWa9CZ5dhcAnSy1giA-YiUEUEIJej0Q/edit?folder=1HuyG3ez13YNEHILs7ky31jL4dIPhU4Sp#slide=id.g27d9461c96e_1_127)  
* Sample code to split a line into words with strtok\! See: [https://docs.google.com/presentation/d/11fwG5voPoGm-1KZAQ-JGaw1uSSSXVL5V8j7dFOuQ2E8/edit?slide=id.g278e77643e5\_0\_314\#slide=id.g278e77643e5\_0\_314](https://docs.google.com/presentation/d/11fwG5voPoGm-1KZAQ-JGaw1uSSSXVL5V8j7dFOuQ2E8/edit?slide=id.g278e77643e5_0_314#slide=id.g278e77643e5_0_314)  
*
