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

int main (int argc, char **argv)
{
    std::vector<std::string> vect;

    int total_files = 0;

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    DIR *point;
    struct dirent *directory;
    point = opendir(cwd);
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

    int columns = w.ws_col/max_size;
    int rows = total_files/columns;
    
    std::vector<std::vector<std::string>> final_vect;

    for (int i = 0; i < columns; i++) {
        std::vector<std::string> helper;
        for (int j = rows * i; j < rows * (i+1); j++) {
            helper.push_back(nvect[i+j]);
        }
        final_vect.push_back(helper);
    }

    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < columns; i++) {
            printf("%-*s", max_size, final_vect[i][j].c_str());
        }
        printf("\n");
    }

    return 0; 
}