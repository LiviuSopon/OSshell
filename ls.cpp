#include <iostream>
#include <typeinfo>
#include <vector>
#include <string>
#include <algorithm>


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>

char *          /* Return name corresponding to 'uid', or NULL on error */
userNameFromId(uid_t uid)
{
    struct passwd *pass;

    pass = getpwuid(uid);
    return (pass == NULL) ? NULL : pass->pw_name;
}

char *          /* Return name corresponding to 'gid', or NULL on error */
groupNameFromId(gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}


int Details(char *path)
{

    struct stat fileStat;
	char *buffer = 0;
	long length, n;
	int file;
	
	 if ((file = open(path, O_RDONLY)) < 0)
	     printf(" !error: file cannot be opened! ");
	   
	 buffer = (char *)malloc (length * sizeof(char));

    if(lstat(path,&fileStat) < 0)    
     {
        return 1;
    }
    
    length = fileStat.st_size;	

    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : (S_ISLNK(fileStat.st_mode)) ? "l" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

    int links = fileStat.st_nlink;
    printf(" %d ", n);

    printf("%d",length);

    close(file); //fclose(f);
    free(buffer);
	
    printf(" %s ", strtok(ctime(&fileStat.st_ctime), "\n"));
    
    return 0;
}


void classify(struct stat st) {

    if(S_ISREG(st.st_mode) && st.st_mode & 0111) {
        printf("*");
        return;
    }

    if(S_ISREG(st.st_mode)) {
        return;
    }

    if(S_ISDIR(st.st_mode)) {
        printf("/");
        return;
    }

    if(S_ISLNK(st.st_mode)) {
        printf("@");
        return;
    }

    if(S_ISFIFO(st.st_mode)) {
        printf("|");
        return;
    }
    
    if(S_ISCHR(st.st_mode)) {
        printf("CHR");
        return;
    }

    if(S_ISBLK(st.st_mode)) {
        printf("BLK");
        return;
    }
    
}

void program(char fldr[], bool a, bool s, bool l, bool F) {

    std::vector<std::string> vect;

    struct stat st;
    
    DIR *point;
    struct dirent *directory;
    point = opendir(fldr);
    if(point) {
        while ((directory = readdir(point)) != NULL) {
            if (a) { // ls -a
                char *name = directory->d_name;
                vect.push_back(std::string(name));
                }
            else {
                if (directory->d_name[0] != '.') {
                    char *name = directory->d_name;
                    vect.push_back(std::string(name));
                }
            }
        }

        sort(vect.begin(), vect.end());

        for (int i = 0; i < vect.size(); i++) {
            char* name = (char*)vect[i].c_str();

                stat(name, &st);

                if (s) {
                int size = st.st_blocks/2; // to better look like the result of ls -s
                printf("%d ", size);
                }

                char path[PATH_MAX];
                snprintf(path, sizeof(path), "%s/%s", fldr, name);

                if (l) {
                Details(path); // long-form aka ls -l
                }

                char spaces[] = {' ', 0};
                if(strpbrk(name, spaces)){
                    printf("'%s'", name);
                }
                else{
                    printf("%s", name);
                }

                if (F) {
                    classify(st); // for ls -F
                }

                printf("\n");
        }
        closedir(point);
    }
}

int main(int argc, char *argv[]) {

    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    int c;

    bool o_a = false;
    bool o_s = false;
    bool o_l = false;
    bool o_F = false;

    while((c = getopt(argc, argv, "aslF")) != -1) {
        switch (c) {
            case 'a': {
                o_a = true;
                break;
            }
            case 's': {
                o_s = true;
                break;
            }
            case 'l': {
                o_l = true;
                break;
            }
            case 'F': {
                o_F = true;
                break;
            }
            default: {
		        exit(0);
            }
        }
    }

    if (optind >= argc) {
        program(cwd, o_a, o_s, o_l, o_F);
    }
    else {
        for (int i = optind; i < argc; i++) {
            if(chdir(argv[i]) < 0) {
                printf("ls: %s: %s\n", strerror(errno), argv[i]);
                continue;
            };
            printf("%s", argv[i]);
            printf("\n");
            char temp[PATH_MAX];
            getcwd(temp, sizeof(temp));
            program(temp, o_a, o_s, o_l, o_F);
            chdir(cwd);
            printf("\n");
        }
    }

    return 0;
}