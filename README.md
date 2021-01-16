# OSshell
College assignment. I would advise everybody not to use this shell.


## Implemented commands:

* help: any argument after calling 'help' is ignored. Prints helpful info.
* version: prints version info. 0.0.1 because I'd advice nobody to use this shell. Also ignores every other argument.
* ls: lists the current directory contents. Implemented args: -l, -s, -a, -F
* tac: concatenate the files, reversed. Implemented args: -b, -s.
* dir: show directory contents on columns vertically sorted. Depends on terminal size at the moment of calling.
    
## ATTENTION:

To compile: first, install dependencies: (Ubuntu examples)
GNU Readline: sudo apt-get install libreadline-dev
libexplain: sudo apt-get install libexplain-dev
    
Then, run the following compilator call for shell.cpp:
g++ shell.cpp -L/usr/local/lib -I/usr/local/include -lreadline -lexplain -o shell.out
    
For the other files, compile with g++ (filename.cpp) -o (outfile.out)
    
## TO RUN LOCAL COMMANDS:

Add './' to (filename.out) as such: ./filename.out\n"
Otherwise, the shell will search in the PATH for the executable.\n")
