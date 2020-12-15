#include <string>
#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

vector<string> cpplit(const string& inp, const string& sep) {
    vector<string> res;

    char c_inp[inp.size()+1];
    strcpy(c_inp, inp.c_str());

    char* p = strtok(c_inp, sep.c_str());
    while(p) {
        res.push_back(string(p));
        p = strtok(NULL, sep.c_str());
    }

    return res;
}

void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	fflush(stdout);
    printf("\n>");
}


int main(int argc, char **argv)
{
    signal(SIGINT, sigintHandler);

    using_history();

    add_history(""); // hacky history initialization to avoid segfaults

    char *input;
    do {
        HIST_ENTRY *previous = history_get(history_length); // get the previous history entry

        input = readline(">"); // get input

        if(strcmp(previous->line, input) != 0) // if input is different from previous history entry
            if(input[0] != '\0') // if input is not empty line
                add_history(input);
        
        auto args = cpplit(input, " ");
        //cout<<args[1];

        char* args_c[args.size()+1];
        args_c[args.size()] = NULL;
        for (int i = 0; i < args.size(); i++) {
            args_c[i] = (char*)args[i].c_str();
        }

        printf("%s\n", args_c[1]);


        if(strcmp(input, "exit\0") == 0) break; // break execution only on "exit"

       // printf("Current line = %s\n", input);
        free(input); // tbh, I read it's good practice to do this
    } while(1);
}
