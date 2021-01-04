#include <stdio.h>

int main() {
    printf( "Implemented commands:\n"
    "*help: any argument after calling 'help' is ignored. Prints helpful info.\n"
    "*version: prints version info. 0.0.1 because I'd advice nobody to use this shell. "
    "Also ignores every other argument.\n"
    "*ls: lists the current directory contents. Implemented args: -l, -s, -a, -F\n"
    "*tac: concatenate the files, reversed. Implemented args: -b, -s.\n"
    "*dir: show directory contents on columns vertically sorted. "
    "Depends on terminal size at the moment of calling.\n\n\n"
    "***!ATTENTION!***\n"
    "To compile: first, install dependencies: (Ubuntu examples)\n"
    "GNU Readline: sudo apt-get install libreadline-dev\n"
    "libexplain: sudo apt-get install libexplain-dev\n\n"
    "Then, run the following compilator call for shell.cpp:\n"
    "g++ shell.cpp -L/usr/local/lib -I/usr/local/include -lreadline -lexplain -o shell.out\n\n"
    "For the other files, compile with g++ (filename.cpp) -o (outfile.out)\n\n"
    "!!!TO RUN LOCAL COMMANDS:!!!\n"
    "Add './' to (filename.out) as such: ./filename.out\n"
    "Otherwise, the shell will search in the PATH for the executable.\n");
}