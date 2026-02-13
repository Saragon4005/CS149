Objectives
----------

*   Get an insight into character encodings, strings, memory in C
    
*   Think of a data structure to store strings
    
*   Learn to measure runtimes of executable programs
    

Summary of what gets turned in:

*   A single .c file **countnames.c**.
    
*   It should compile successfully on the zyLab environment.
    
*   Check it passes the tests supplied to you.
    
*   Think of three of your own tests and check it passes them.
    

count of names
--------------

For this assignment we will be implementing an operation that is missing from the the default Linux libraries, which counts for all the names in files how many times each name appears. Sometimes you have a file of names (see [names file examples under A1/test](https://sjsu.instructure.com/courses/1619829/files/folder/code/a1_2/a1_test), which has one name per line with names newline-separated). We want to write a program that counts all the individual names for us. We are going to write a utility that is missing from linux: **countnames**.  To create the **countnames.c** binary, you'll be creating a single source file, **countnames.c**, which will involve writing C code to implement it. You could write a complicated function with awk, but the syntax is hard to remember, thus we will write **countnames** with a clean interface.

_Simplifying assumptions:_ You are taking a text file stream as input with a name on each line, and names are separated by newlines. Assume each line has one name. A name can occur many times in the file. For now assume you are just dealing with up to 100 distinct names (though each name can be repeated in the input file many times), since you would need dynamic memory allocation for expanding an array to deal with more names (which we'll see later). You want to read the names in and count the occurrences of each. Since we want to be flexible on the type and size of characters we can handle, you can assume Ascii code and you _expect a file to contain simple characters of Ascii values from 0-127_. **Assume a name can be up to 30 characters long**. In case a file has no names the program will simply exit. You can assume that the input file contains only valid characters and rows are separated by newlines; the input file may contain a few empty lines as well (which you will ignore as described below).

It is ok if this is slow for now (since you would need dynamic data structures like hash tables to make it fast, which we'll see later). If you want, you can already use a hash table. Template code for a C hash table and linked list are provided under folder [code/a4](https://sjsu.instructure.com/courses/1619829/files/folder/code/a4). **You could use either a hash table or linked list already for storing info on name counts, since you will re-use it later for other assignments. A hash table will be faster than a sorted linked list, which will be faster than an unsorted array.**

For example, [names.txt](https://sjsu.instructure.com/courses/1619829/files/85702917?wrap=1) [Download names.txt](https://sjsu.instructure.com/courses/1619829/files/85702917/download?download_frd=1)contains a list of names. _Your program takes as its_ _command-line arguments_ _the filename_. Then it prints the count of individual names. A typical usage is as follows (argc == 2):

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   $ ./countnames names.txt   `

which will output to **stdout** something like (the order will be dependent on your code):

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   Nicky: 1Dave Joe: 2John Smith: 1Yuan Cheng Chang: 3   `

_If there is an empty line in the input file (strlen(line) == 0) your program ignores it for the count purposes_ and prints a message to stderr (_You can use_ _**strlen(line) < 1**_ _to check if a line is empty_). In other words, empty lines are not counted and the program counts the non-empty lines only. In the example above, if your input file contained empty lines you should also print to **stderr** warning messages like this:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   Warning - Line 2 is empty.Warning - Line 5 is empty.   `

Note: a non-empty line with one or more whitespaces are considered part of a name. E.g. a line with just a single space is a name, a line with two spaces is another name, etc. "Tom Wu" is one name, so a name can contain whitespaces (a valid ASCII char).

The names are separated by newlines ('\\n' is Unix's line separator) and you should replace the newline with NULL ('\\0' or zero), as described in the notes below, since the newline is not part of the name.

As you see above, the input filename is given on the command line, since the filename can have any name. You can use argc/argv to read the filename as a command-line argument. **Don't hardcode the input filename as "names.txt" in your code!**

If you want to see just the stderr messages, you can redirect the stdout to /dev/null. For example:   **$ ./countnames names.txt > /dev/null** 

If you want to ignore the stderr (see just the stdout) messages, you can redirect the stderr to /dev/null. For example:    **$ ./countnames names.txt 2> /dev/null** 

The "**./**" before the **countnames** above is a UNIX thing; it just tells the system which directory to find the **countnames** binary in (in this case, in the "." (dot) directory, which means the current working directory). On the zy terminal you may need to prefix the filenames with "test/" to specify the subdirectory location.

It is a good idea to try running your program once with sorting "**./countames names.txt | sort**" to see the output names sorted alphabetically. You can do this so the output doesn't get contaminated with stderr messages: "**./countames names.txt 2> /dev/null | sort**".

To compile this program in the zyLab console, or in any Unix shell terminal, you can do the following:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`$ gcc -o countnames countnames.c -Wall -Werror  $` 

This will make a single _executable binary_ called **countnames**, which you can then run as above.

Submission

Besides the given test cases, you must also provide at least 3 test cases you devised on your own (as separate text files you upload under the A1 test/ subdirectory).Submit a README.md file, showing at the top your student name(s), and showing exactly how to:- compile your code- run your compiled executable on each test case- show what output is expected for each test case- mention for each test case what situation/edge case it is testing for (all cases should test for something).

Additionally, at the bottom of the README.md file include these 3 sections:

\# Lessons learned : describe what you learned from this assignment (anything at all).# References : include citations (with links) to any source of information you found and used. For example, you could cite/link a forum discussion (stackoverflow), or the course Canvas/slides, or a website, or a book. In case you used a chatbot, you should make the chat public and link it.# Acknowledgements : mention any help you got from someone/thing. In case noone helped you directly, you may mention the course embedded tutor, specific people from the discord channel, class time, google, or an AI tool.

Also submit a pdf file "output.pdf" showing screenshots of your compilation and execution on all the testcases. The graders may compile the assignment code and test it, but if your output.pdf looks convincing it could save them some time.In zyLab you need to click "Submit" to submit your work.

grading
-------

submitted code compiles cleanly (no errors/warnings, check with **gcc -o countnames countnames.c -Wall -Werror**)10%overall requirements of assignment are satisfied20%output correctly for test cases with test files provided under the A1 test/ subdirectory30%program flow is understandable10%number of lines in input file that can be processed is not limited, assume it can be up to a few thousand lines (ok if it's slow for now)10%code is commented and indented (use of whitespace), same as all assignments, see [guide](https://sjsu.instructure.com/courses/1619829/pages/commenting-and-documentation-for-homework-assignments)10%submit **countnames.c** with a README.md and output.pdf10%

submission
----------

The submission contains **countnames.c** and it doesn't need any other files to compile (unless you also put code in other files). The testcase files given are small in size and are under the test/ subdirectory, you should put your own test cases there too. Remember to show in README.md how to run the test cases and expected output. Also provide screenshots in output.pdf to assist the graders.

further details
---------------

*   C does not have a dedicated string data type like Java. Strings are char arrays and the end is denoted by a null character  '\\0' (or NULL or 0). Thus, you are strongly advised to initialize the memory buffers you declare to 0's.
    
*   When you read a new line from stdin with fgets, it will contain a newline '\\n'. As shown in the C slides, you should replace it with NULL (or 0 or '\\0') with this command: if (buf\[strlen(buf) - 1\] == '\\n') buf\[strlen(buf) - 1\] = '\\0'; Other methods to replace newlines with NULL can be seen on this page: [https://aticleworld.com/remove-trailing-newline-character-from-fgets/Links to an external site.](https://aticleworld.com/remove-trailing-newline-character-from-fgets/)
    
*   Text editors add new lines ('\\n') at the end of text files as a rule. A reason is so programs can read the last line with fgets or getline. An '\\n' denotes ending a line. If a text editor shows a visible blank line at the end of file it is often just text editor behavior, it isn't necessarily because there is an actual line there unless it has the newline. You can check all the test files with: od -a filename (works on MacOS too) and verify there is a nl at the end. Example:
    

% od -a names.txt0000000 N i c k y nl nl D a v e sp J o e nl....

*   Note: input file names.txt appears to have an empty line in the end. You may check the exact file content using the od command, as shown above. If you download names.txt and open it in a text editor, there appeard to be an empty line in the end. This shouldn't bias your result. An empty line should be ignored by the program.
    
*   A clarification: Your code will be tested as shown in the assgt description above. So you can assume one file is passed through argc/argv on cmd line. This case is **argc == 2** (giving a filename after the executable on cmd line). BUT there could be many files passed with cat from **stdin**, like this: cat file1 file2 file3 | ./countnames. This second case is **argc == 1** (reading all names from **stdin**). So, if no filename is given (argc == 1) then your program can take the stream of names from stdin, as follows:
    

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   cat names.txt | ./countnamescat names.txt names1.txt names2.txt | ./countnames./countnames < names.txt   `

or:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   ./countnames  Jenn Xu  Tom Wu....  ^D   <--Ctrl-D is the EOF signal, which terminates your input   `

Both of the above are ways to stream the input names from stdin to the program. You can compare your result to a correct counts result with: **cat filenames | sort | uniq -c**

more details
------------

*   In all non-error cases, **countnames** should exit with status code 0, usually by returning a 0 from **main()** (or by calling **exit(0)**). This is called the exit code of a program and is different from what your program prints to stdout/stderr.
    
*   You can check the exit code from the previously executed program in the zyLab console (or in any Unix shell terminal) by entering: **echo $?**
    
*   If _no file_ or other parameters are specified on the command line, or an empty file with no numbers is given, **countnames** should just exit and return 0.  A user may run it with no parameters to check if the program is installed.
    
*   If the program tries to **fopen()** a file and fails, it should print the exact message "error: cannot open file" (followed by a newline) and exit with status code 1.
    

You may need to learn how to use a few library routines from the C standard library (often called **libc**) to implement the source code for this program, which we'll assume is in a file called **countnames.c**. All C code is automatically linked with the C library, which is full of useful functions you can call to implement your program. Learn more about the C library [hereLinks to an external site.](https://en.wikipedia.org/wiki/C_standard_library) and perhaps [hereLinks to an external site.](https://www-s.acm.illinois.edu/webmonkeys/book/c_guide/).

For this project, we recommend using the following routines to do file input and output: **fopen()**, **fgets()**, **fscanf()**, **fclose()**. Whenever you use a new function like this, the first thing you should do is read about it -- how else will you learn to use it properly? 

On UNIX systems, the best way to read about such functions is to use what are called the **man** pages (short for **manual**). In our HTML/web-driven world, the man pages feel a bit antiquated, but they are useful and informative and generally quite easy to use.

To access the man page for **fopen()**, for example, just type the following at your UNIX shell prompt:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   prompt> man fopen   `

Then, read! Reading man pages effectively takes practice; why not start learning now?

We will also give a simple overview here. The **fopen()** function "opens" a file, which is a common way in UNIX systems to begin the process of file access. In this case, opening a file just gives you back a pointer to a structure of type **FILE**, which can then be passed to other routines to read, write, etc.

Here is a typical usage of **fopen()**:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   FILE *fp = fopen("numbers.txt", "r");  if (fp == NULL) {      printf("cannot open file\n");      exit(1);  }   `

A couple of points here. First, note that **fopen()** takes two arguments: the _name_ of the file and the _mode_. The latter just indicates what we plan to do with the file. In this case, because we wish to read the file, we pass "r" as the second argument. Read the man pages to see what other options are available.

Second, note the _critical_ checking of whether the **fopen()** actually succeeded. This is not Java where an exception will be thrown when things goes wrong; rather, it is C, and it is expected (in good programs, i.e., the only kind you'd want to write) that you always will check if the call succeeded. Reading the man page tells you the details of what is returned when an error is encountered; in this case, the macOS man page says:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`Upon successful completion fopen(), fdopen(), freopen() and fmemopen() return  a FILE pointer.  Otherwise, NULL is returned and the global variable errno is  set to indicate the error.` 

Thus, as the code above does, please check that **fopen()** does not return NULL before trying to use the FILE pointer it returns.

Third, note that when the error case occurs, the program prints a message and then exits with error status of 1. In UNIX systems, it is traditional to return 0 upon success, and non-zero upon failure. Here, we will use 1 to indicate failure.

Side note: if **fopen()** does fail, there are many reasons possible as to why. You can use the functions **perror()** or **strerror()** to print out more about _why_ the error occurred; learn about those on your own (using ... you guessed it ... the man pages!).

Once a file is open, there are many different ways to read from it. We suggest **fscanf()** or **fgets()**, which is used to get input from files, one line at a time.

Fgets reads in a string per line: [https://www.tutorialspoint.com/c\_standard\_library/c\_function\_fgets.htmLinks to an external site.](https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm) 

scanf uses **any whitespace as a delimiter**, so if you just say scanf("%d", &var) it will skip any whitespace and then read an integer (digits up to the next non-digit) and nothing more.

To print out file contents, you may use **printf()**. For example, after reading in a line with **fgets()** into a variable **buffer**, you can just print out the buffer as follows:

Plain textANTLR4BashCC#CSSCoffeeScriptCMakeDartDjangoDockerEJSErlangGitGoGraphQLGroovyHTMLJavaJavaScriptJSONJSXKotlinLaTeXLessLuaMakefileMarkdownMATLABMarkupObjective-CPerlPHPPowerShell.propertiesProtocol BuffersPythonRRubySass (Sass)Sass (Scss)SchemeSQLShellSwiftSVGTSXTypeScriptWebAssemblyYAMLXML`   printf("%s", buffer);   `

Note that you should _not_ add a newline (\\n) character to the printf(), because that would be changing the output of the file to have extra newlines. Just print the exact contents of the read-in buffer (which, of course, many include a newline).

Finally, when you are done reading and printing, use **fclose()** to close the file (thus indicating you no longer need to read from it).