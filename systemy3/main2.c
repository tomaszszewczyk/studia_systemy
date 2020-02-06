#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute(char * file)
{
    FILE * fd = fopen(file, "r");
        if(fd == NULL) {
        printf("Error while opening file\n");
        exit(-1);
    }

    char command[1000];

    while(fgets(command, 1000, fd) != NULL) {
        char * args[16];
        size_t count = 0;

        printf("%s\n", command);

        char * arg = strtok(command, " ");

        for(int i = 0; i < 16; i++) {
            if(arg != NULL) {
                args[i] = arg;
                count = i + 1;
                printf("\t%s\n", arg);
                arg = strtok(NULL, " ");
            }
            else {
                break;
            }
        }

        pid_t pid = fork();

        if(pid == 0) {
            int status;
            waitpid(pid, &status, 0);

            if(status != 0) {
                printf("Error at: %s\n", command);
                exit(1);
            }
        } else {
            execvp(args[0], args + 1);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("Too few args");
    }

    execute(argv[1]);
}