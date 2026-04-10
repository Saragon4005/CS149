## Objectives

* Understand how processes can communicate through shared memory

* Learn how to use the mmap() system call to allocate dynamic shared memory

* Learn how multiple processes can write results into a shared global memory region

* Understand how to divide shared memory into process-specific subspaces

* Extend the parallel name counting system developed in Assignments 1–3

# Background

In **Assignment 1**, you implemented the countnames utility that reads a file containing names and counts how many times each name appears.

In **Assignment 2**, you introduced **parallel processing**. The shell program spawned multiple child processes so that multiple files could be processed simultaneously.

In **Assignment 3**, child processes communicated their results back to the parent using **pipes,** so that the parent process could aggregate the results.

In this assignment, we will replace **pipe communication with shared memory using the mmap() system call**. Instead of sending results through pipes, child processes will **write their name counts directly into a shared memory region that represents global memory**. The parent process will then **combine the results stored in this memory**.

# Tasks

You will extend your **Assignment 3 implementation**.

Programs involved:

* shell.c

* countnames.c

The overall behavior of countnames remains the same as in Assignment 3.

Example usage:

./countnames names1.txt names2.txt names3.txt

Each file will still be processed by a **separate child process**.

However, instead of sending results through **pipes**, the child processes will **write their results into shared memory**.

# Shared Memory using mmap

In this assignment, shared memory will be created using the mmap() system call.

The parent (shell) process will allocate a **large shared memory region** that will act as **global memory**.

Example:

This shared memory region will be accessible by both the **parent and child processes**.

The memory region should be **large enough to store the results for all input files**.

# Expanding the NameCount Array with mmap

In previous assignments, the array used to store name counts was stored in **regular process memory**.

In this assignment, you will store the namecount array using mmap so that **multiple processes can access and write to the same memory region**.

The memory returned by mmap() is a pointer of type void\*.

In order to use it as an array of structures, you must **cast it to the appropriate pointer type**.

Example:

NameCountData \*data = (NameCountData \*) GLOBAL;

This allows you to access the memory using array syntax such as: data\[i\].

# Memory Layout

The parent (shell) process will create a large shared memory region named **GLOBAL**.

This region will be divided into several sections.

Each child process will be assigned its **own region within GLOBAL**. Children must not write outside their assigned region of memory. Doing so may corrupt the results written by other processes.

The parent (shell) process will also allocate a **summation region** where the final combined results will be stored.

Example layout:

# NameCount Array Termination

Each region of the shared memory will store an array of NameCountData structures.

All arrays should be initialized to NULL entries before use.

A record with a name of length 0 (NULL at index 0) will be used to indicate the end of valid data in that region.

When the parent process aggregates the results, it will stop reading entries once it encounters a name with **length 0**.

# Creating mmap Subspaces

The parent (shell) process will divide the **GLOBAL region into multiple subspaces**.

Each child process will receive an **offset pointing to its assigned memory region**.

These regions must **not exceed the total size of the GLOBAL memory region**.

For example, each child's starting address of its assigned region may be computed using pointer arithmetic:

This calculation determines the **starting location of the memory region assigned to a specific child process**.

Each child process should **store its namecounts within its assigned region of the shared memory**.

**Child Process Responsibilities**

Each child process will:

* Read its assigned input file

* Compute the name counts for that file

* Write the results into its assigned region of the **GLOBAL shared memory**

* Exit with status code 0 if successful

Instead of sending data through **pipes**, each child writes **directly to its assigned region of the shared memory**.

**Parent Process Responsibilities**

The parent process (shell.c) will:

* Create the **GLOBAL shared memory region** using mmap

* Divide this memory into **regions for each child process**

* Spawn child processes using fork() and execvp()

* Wait for all children to complete execution using wait()

* Read the results from each child's memory region

* Combine the name counts in the **summation region**

* Print the final combined name counts

## Aggregation of Results

After all child processes finish computing the name counts, the results must be combined into a final aggregated set of name counts. Two implementation approaches are possible.

#### Option 1 — Parent Aggregates Results (easier)

Each child process wrote its computed name counts into its assigned region of the GLOBAL shared memory.

So after child processes complete execution 1-2, the parent process reads the results stored in each child’s region and combines them into the **summation region** of GLOBAL.

The parent then prints the final aggregated name counts.

#### Option 2 — Children Aggregate into GLOBAL (harder)

Each child process computes the name counts for its assigned file and directly updates the **shared summation region** inside GLOBAL.

In this approach, the children contribute their results directly to the global aggregated data structure.

This approach must ensure that writes to the shared region do not overwrite or corrupt existing results, thus this approach is harder.

# Example Execution

Example command:

./countnames names1.txt names2.txt names3.txt

Execution steps:

1. Parent creates GLOBAL shared memory using mmap()

2. Parent divides GLOBAL into child-specific regions

3. Parent spawns child processes using fork()

4. Each child processes one input file

5. Each child writes its results into its assigned region in GLOBAL

6. Parent waits for all children to finish using wait()

7. The results are aggregated (as described above)

8. Final combined name counts are printed

# Example Code for GLOBAL Shared Memory

The following example illustrates how a large mmap region can be created and how a subspace inside that region can be addressed.

This example is **only meant to demonstrate how shared memory regions may be organized**. Your implementation does **not need to match this code exactly**.

This example demonstrates how **offsets within a large mmap region** can represent **different memory subspaces**.

### Suggested Solution (easier)

You already implemented in A2-A3 the child processes' writing their results to PID.out files (PID is process id). So you can modify to write your array of NameCount structs to a file with mmap.  Then the parent (after a child finishes) can read PID.out (it knows the pid of the completed child from wait) into the child's mmap region and uses it to update its combined summation region. Here is an [example](https://sjsu.instructure.com/courses/1619829/files/86661965?wrap=1) [Download example](https://sjsu.instructure.com/courses/1619829/files/86661965/download?download_frd=1)of how to read and write an array to file with mmap.

### Other Solution (harder but better. If you do this solution it counts as Bonus 1% \[submit under the Bonus assignment page\])

While after fork() the children inherit the shared memory mapping and variables, after exec those get lost, which is the main challenge. Nevertheless, the file descriptors get preserved after exec, and it would be great for the parent shell process to control the mmapped space globally. To access memory mapping after an exec, the new program must **re-connect** to the shared memory space either by using a file descriptor returned by shm\_open (ensure FD\_CLOEXEC is cleared via fcntl) ; or pass the name of the shared object to the new process with argv. Here is a code [sample](https://sjsu.instructure.com/courses/1619829/files/86666065?wrap=1) [Download sample](https://sjsu.instructure.com/courses/1619829/files/86666065/download?download_frd=1)of how parent and child can communicate through a shared memory space even after exec.

# Submission (same as A1–A3)

Besides the given test cases, you must also provide **at least 3 test cases you devised on your own** (as separate text files under the test/ directory). **Your A4 test cases can be the same ones as A1-A3, or you can add more. For simplicity assume up to 3 input files (no need to test with more).**

Submit the following files:

shell.c

countnames.c

README.md

output.pdf

Your README.md must include:

* student name(s)

* instructions on how to compile the programs

* instructions on how to run the test cases

* expected output for each test case

* explanation of what each test case is testing

At the end of the README include the following sections:

\# Lessons learned

\# References

\# Acknowledgements

Also submit a pdf file "output.pdf" showing screenshots of your compilation and execution on all the testcases. The graders may compile the assignment code and test it, but if your output.pdf looks convincing it could save them some time.  Remember, in zyLab you need to click "Submit" to submit your work.

# Compilation

Example compilation commands:

gcc -o shell shell.c -Wall -Werror

gcc -o countnames countnames.c -Wall -Werror

Your code should compile **without warnings**.

# Grading

| Category | Weight |
| --- | --- |
| overall requirements are satisfied | 20% |
| compiles cleanly (no errors/warnings, use **gcc -o shell shell.c -Wall -Werror ; gcc -o countnames countnames.c -Wall -Werror**) | 5% |
| correct use of fork() and exec() | 10% |
| correct use of wait() | 10% |
| correct use of mmap shared memory | 20% |
| correct division of memory into child regions | 10% |
| correct aggregation of name counts | 15% |
| processes run in parallel | 5% |
| code readability and comments | 3% |
| submission files and documentation | 2% |
## Submission

The submission contains **shell.c** and **countnames.c** and it doesn't need any other files to compile (unless you also put code in other files). The testcase files should be under the test/ subdirectory. Show in README.md how to compile and run the test cases and expected output.

# Optional Reading (not required)

On systems that contain both a **CPU and a GPU**, memory is organized into different spaces. The CPU has access to **system RAM**, while the GPU contains its own device memory, which may include **global memory** and **constant memory**.

For this assignment you are **simulating the idea of GPU global memory** using shared memory created with mmap().

While the standard mmap system call does not directly transfer data between CPU and GPU memory, similar memory mapping principles are used in GPU programming frameworks.

Examples include:

* CUDA mapped pinned memory

* CUDA unified memory

These systems allow CPU and GPU to access memory regions in ways conceptually similar to **shared memory between processes**.

**Reference:**
