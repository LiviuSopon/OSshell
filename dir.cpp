#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <math.h>

void program(char fldr[]) {
    
    std::vector<std::string> vect;

    int total_files = 0;

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    DIR *point;
    struct dirent *directory;
    point = opendir(fldr);

    if(point) {
        while ((directory = readdir(point)) != NULL) {
            if (directory->d_name[0] != '.') {
                total_files++;
                char *name = directory->d_name;
                vect.push_back(std::string(name));
            }
        }
    }

    sort(vect.begin(), vect.end());

    int max_size = 0; // biggest name size

    std::vector<std::string> nvect;

    for (auto s : vect) {
        for (std::string::size_type i = 0; i < s.size(); i++) {
            if (!strchr("_.{}()[]", s[i]))
                if(!(isalnum(s[i]) && s[i] != ' ')) {
                    s.insert(i, 1, '\\');
                    i++;
                }
        }
        nvect.push_back(s);
        if(max_size < s.length())
            max_size = s.length();
    }

    max_size++;

    int rows;
    
    int columns = w.ws_col/max_size;
    if (columns) rows = ceil(float(total_files)/float(columns));

    if(columns >= total_files) {
        for (int i = 0; i < total_files; i++)
            printf("%s ", nvect[i].c_str());
        printf("\n");
    }

    else if(columns == 0) {
        for (int i = 0; i < total_files; i++) {
            printf("%s", nvect[i].c_str());
            printf("\n");
        }
    }

    else {
        std::vector<std::vector<std::string>> final_vect;
        for (int i = 0; i < columns; i++) {
            std::vector<std::string> helper;
            for (int j = rows * i; j < rows * (i+1); j++) {
                    try{
                        helper.push_back(nvect[j]);
                    }
                    catch(std::logic_error){
                        continue;
                    }
                }
            final_vect.push_back(helper);
        }
        for (int j = 0; j < rows; j++) {
            for (int i = 0; i < columns; i++) 
                printf("%-*s", max_size, final_vect[i][j].c_str());
            printf("\n");
        }
    }
}

int main (int argc, char **argv)
{
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    int c;

    while((c = getopt(argc, argv, "")) != -1) {
        switch (c) {
            default: {
		        exit(0);
            }
        }
    }

    if (optind >= argc) {
        program(cwd);
    }
    else {
        for (int i = optind; i < argc; i++) {
            if(chdir(argv[i]) < 0) {
                printf("Dir: %s: %s\n", strerror(errno), argv[i]);
                continue;
            };
            printf("%s", argv[i]);
            printf("\n");
            char temp[PATH_MAX];
            getcwd(temp, sizeof(temp));
            program(temp);
            chdir(cwd);
        }
    }

    return 0; 
}