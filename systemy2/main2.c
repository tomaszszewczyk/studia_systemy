#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(char * file)
{
    FILE * fd = fopen(file, "r");
        if(fd == NULL) {
        printf("Error while opening file\n");
        exit(-1);
    }

    char command[1000];

    while(fgets(command, 1000, fd) != NULL) {
        char * args[16] = {0};
        size_t count = 0;

        command[strlen(command) - 1] = 0;
        printf("%s\n", command);

        char * arg = strtok(command, " ");

        for(int i = 0; i < 16; i++) {
            if(arg != NULL) {
                args[i] = arg;
                count = i + 1;
                arg = strtok(NULL, " ");
                printf("\t%s\n", args[i]);
            }
            else {
                break;
            }
        }

        pid_t pid = fork();

        if(pid != 0) {
            int status;
            waitpid(pid, &status, 0);

            if(status != 0) {
                printf("[Parent] Error at: %s, %d\n", command, errno);
                exit(1);
            }
        } else {
            execvp(args[0], args);
            printf("[Child] Error at: %s, %d\n", command, errno);
            exit(1);
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