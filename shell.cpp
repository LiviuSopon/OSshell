#include <string>
#include <vector>
#include <iostream>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <libexplain/execvp.h>

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

void sigintHandler(int sig_num) {
	signal(SIGINT, sigintHandler);
	fflush(stdout);
    printf("\n>");
}

void ex(const char* command, char *const  arguments[]) {
    int pid = fork();
    if (pid < 0) {
        printf("Yo, dawg, there's been an error.");
        exit(-1);
    }
    else if (pid == 0) {
    // Standard error reporting == BAD

     /* execvp(command, arguments);
        int err = errno;
        fprintf(stderr, "%d", err);
        exit(EXIT_FAILURE);
    */

    // Using the (quite aggressive) libexplain, instead.
        explain_execvp_or_die(command, arguments);

    }
    else {
        wait(NULL);
    }
}

void single_cmd(const string& strin){
    auto args = cpplit(strin, " ");
    char* cmd = (char*)args[0].c_str(); // first argument for execvp
    char* args_c[args.size()+1];
    args_c[args.size()] = NULL;
    for (int i = 0; i < args.size(); i++) {
        args_c[i] = (char*)args[i].c_str(); // second argument for execvp
    }
    ex(cmd, args_c);
}

void through_pipe(vector<string> comms) {
    int pip[2];
    int pid;
    int in = 0;

    for(int i = 0; i<comms.size(); i++) {
        pipe(pip);
        pid = fork();
        if (pid < 0) {
            exit(-1);
        }
        else if (pid == 0) {
            dup2(in, 0);
            if(i < comms.size() - 1) {
                dup2(pip[1], 1);
            }
            close(pip[0]);
            auto args = cpplit(comms[i].c_str(), " ");
            char* cmd = (char*)args[0].c_str();
            char* args_c[args.size()+1];
            args_c[args.size()] = NULL;
            for (int i = 0; i < args.size(); i++) {
                args_c[i] = (char*)args[i].c_str();
            }
            explain_execvp_or_die(cmd, args_c);
            exit(EXIT_FAILURE);
        }
        else {
            wait(NULL);
            close(pip[1]);
            in = pip[0];
        }
    }
}

int main(int argc, char **argv)
{
    char cwd[PATH_MAX];
    char path[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        strcpy(path, cwd);
        printf("Current working dir: %s\n", path);
        } 
   else {
       perror("getcwd() error");
       return 1;
       }

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
        
        if(strcmp(input, "exit\0") == 0) break; // break execution only on "exit"

        auto commands = cpplit(input, "|");
        
        if(commands.size() == 1){
            single_cmd(commands[0].c_str());
        }
        else if(commands.size() > 1){
            through_pipe(commands);
        }

        free(input); // tbh, I read it's good practice to do this
    } while(1);
}
