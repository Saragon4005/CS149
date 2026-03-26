# Assignment 3 Instructions

**Objectives:** Understand how processes get executed in parallel.

* Learn how processes communicate through pipes or mmap

You will extend your A2 implementation, such that each child writes data back to the parent (the shell). For A3 the parent shell.c will need to sum the count of names over all the files. This extends A2 in that the parent will aggregate all results in the end by summing them up (you can still give multiple files as command-line arguments).

The parent process (shell) in the end will combine the results to compute a total names' count, and print the names' count to stdout in the end. If a parent process (shell) has no child process (countnames) then wait(..) returns "-1", thus you can continue until wait(..) returns -1 (see wait examples in slides and sample code given1). For example, Child 1 returns Alexa: 1, John: 2 ; Child 2 returns John: 1, Jas: 5 ; Parent prints Alexa: 1, John: 3, Jas: 5.

You will need to modify the program countnames.c as described below. The exit codes are the same as A1: if a child can open its input names file and count the names, then it returns 0 as exit code. For communication between the child processes (countnames) and the parent (shell) you could implement either of these two options:

Option 1: use a pipe

To communicate between parent and child processes you may use pipe(). The [code/ipc](https://sjsu.instructure.com/courses/1619829/files/folder/code/ipc) directory contains examples of using fork, pipe and wait, which you can consult for help.

The child can send data through the write end of the pipe it inherits from its parent (the shell). Each child will inherit the pipe from its parent (shell1) and write the result back to the parent for processing.

The child can use dup or dup2 to replace stdout with the write end of the pipe (also could replace stdin with the read end of the pipe, but not needed). The parent shell.c could call dup or dup2 to replace stdout with the write end of the pipe, after creating the pipe and before forking/spawning the child (since the child inherits files from the parent), so the child can send to the parent by printing to stdout (in that case after the child completes you would need to reinstate stdout in the parent for printing results).

For future extensibility reasons, you can use this structure to return data from the child:

```c
typedef struct {
    char name[30];
    int count;
} NameCountData;
```

You could use this fixed-size header structure as the first element of all your pipe communication, which explicitly identifies the type and size of the data being sent (it would allow someone in the future to send and receive different struct types through a pipe):

```c
typedef enum {
    TYPE_NAMECOUNT,
    TYPE_B // there is a possibility to extend with more types in the future
} MessageType;

typedef struct {
    MessageType type;
    size_t size; // Size of the following payload
} MessageHeader;
```

The sender (child) writes data to the pipe by writing the header first and then the structs of namecounts:

```c
void write_struct_namecount(int fd, NameCountData *data) {
    MessageHeader header;
    header.type = TYPE_NAMECOUNT;
    header.size = sizeof(NameCountData);
    write(fd, &header, sizeof(header)); // Write the header
    write(fd, data, sizeof(NameCountData)); // Write the payload
}
```

The receiver (parent) reads data from the pipe by reading the header first and then uses a switch statement to read the correct amount of data into the appropriate struct. The read function may require a loop to ensure all expected bytes are received:

```c
void read_from_pipe(int fd) {
    MessageHeader header;
    // Loop on read to ensure all header bytes arrive
    if (read(fd, &header, sizeof(header)) <= 0) {
        // Handle error or end of pipe
        return;
    }
    switch (header.type) {
        case TYPE_NAMECOUNT: {
            NameCountData dataNC;
            read(fd, &dataNC, header.size);
            // Process NameCountData data
            printf("Received NameCountData: name=%s, count=%d\n", dataNC.name, dataNC.count);
            break;
        }
        case TYPE_B: {
            // This case is for a possible extension in future to process other Struct data
            break;
        }
        default:
            // Handle unknown type error
            fprintf(stderr, "Unknown message type received: %d\n", header.type);
    }
}
```

Note: to make the assignment easier, it is OK if you assume all data sent is of type NameCountData, so you don't need to send a MessageHeader before NameCountData. Thus you won't lose points if you don't send a MessageHeader before NameCountData. The MessageHeader could be useful for making the pipes generic to any data, but this is not a requirement for this assignment.

Option 2: use mmap (Assignment 4)

You could implement the above using mmap, as well.

Mention in your README if you used a pipe or mmap.

\------

Submission (same as A1-A2)

Besides the given test cases, you must also provide at least 3 test cases you devised on your own (as separate text files you upload under the A3 test/ subdirectory). Your A3 test cases can be the same ones as A1-A2, or you can add more. For simplicity assume up to 3 input files (no need to test with more).

Submit a README.md file, showing at the top your student name(s), and showing exactly how to:- compile your code- run your compiled executable on each test case- show what output is expected for each test case- mention for each test case what situation/edge case it is testing for (all cases should test for something).

Additionally, at the bottom of the README.md file include these 3 sections (same as A1-A2):

\# Lessons learned : describe what you learned from this assignment (anything at all).# References : include citations (with links) to any source of information you found and used. For example, you could cite/link a forum discussion (stackoverflow), or the course Canvas/slides, or a website, or a book. In case you used a chatbot, you should make the chat public and link it.# Acknowledgements : mention any help you got from someone/thing. In case noone helped you directly, you may mention the course embedded tutor, specific people from the discord channel, class time, google, or an AI tool.

Also submit a pdf file "output.pdf" showing screenshots of your compilation and execution on all the testcases. The graders may compile the assignment code and test it, but if your output.pdf looks convincing it could save them some time.  Remember, in zyLab you need to click "Submit" to submit your work.

## Grading

overall requirements are satisfied (such as spawning of processes)20%compiles cleanly (no errors/warnings, use **gcc -o countnames countnames.c -Wall -Werror; gcc -o shell shell.c -Wall -Werror**)5%correctly uses fork() and exec()10%correctly uses wait() (or a variant of wait)10%correct use of file handles (in parent, and children, possibly with dup/dup2)10%correctly uses a version of pipe for communication between parent-child processes (or mmap)20%all processes run in parallel and are spawned correctly 310%an output and error file is still created for each command (same as A2)5%code is commented and indented (use of white space), same as A15%submit **shell.c** and **countnames.c** with a README.md5%

## Submission

The submission contains **shell.c** and **countnames.c** and it doesn't need any other files to compile (unless you also put code in other files). The testcase files should be under the test/ subdirectory. Show in README.md how to compile and run the test cases and expected output.

1 Sample code to parse error code and signal of a completed process is in the slides! See:  [https://docs.google.com/presentation/d/1tFAJHE88J3ylpWa9CZ5dhcAnSy1giA-YiUEUEIJej0Q/edit?folder=1HuyG3ez13YNEHILs7ky31jL4dIPhU4Sp#slide=id.g27d9461c96e\_1\_127Links to an external site.](https://docs.google.com/presentation/d/1tFAJHE88J3ylpWa9CZ5dhcAnSy1giA-YiUEUEIJej0Q/edit?folder=1HuyG3ez13YNEHILs7ky31jL4dIPhU4Sp#slide=id.g27d9461c96e_1_127)

2 [https://www.tutorialspoint.com/c\_standard\_library/c\_function\_perror.htmLinks to an external site.](https://www.tutorialspoint.com/c_standard_library/c_function_perror.htm)

3 Same as in assignment 2, you want your processes to get spawned in parallel, then the parent waits for any process to finish. Thus you don't have a bottleneck of a short process waiting behind a long process to finish. Re-read from A2: "An objective of this assignment is to learn to program multiple processes in such a way that a slow process won't cause the faster processes to stall (eliminate bottlenecks). For example, assume the first file involves lots of lines (slow), while the second file contains only one line. The _wrong_ way to implement the code would be for your second process to wait for the first process (which does more processing) to completely run through completion before the second process even starts. That would resemble the situation where a line-up of cars are trailing a slow-speed tractor on a single-lane road (wrong sequence of calls: fork1; wait1; fork2; wait2...). For this reason, the parent should fork processes that will run independently and then wait for any process to finish (the OS will handle the context switching between processes). See the slides for the code to use."

4 Multiple child processes can write to the same pipe in Unix-like OSs. After a parent process creates a pipe using pipe() and then uses fork()to spawn child processes, the child processes inherit copies of the file descriptors pointing to the same pipe. Data written to the write end of the pipe by any child process can be read from the read end by the parent process. Writes below 4096 bytes (defined by PIPE\_BUF) are guaranteed to be atomic, meaning the data from different child processes will not be interleaved or corrupted. You could also use one pipe per child if you prefer.

This tool needs to be loaded in a new browser window
