#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

        pid_t pids[16];
        int pidi = 0;

        int pipefd[2];

        for(int i = 0; i < count;) {
            pid_t pid = fork();
            pids[pidi++] = pid;

            if(pid == 0) { // child
                if(i != 0) {
                    dup2(pipefd[1], STDIN_FILENO);
                }

                pipe(pipefd);
                dup2(pipefd[0], STDOUT_FILENO);

                char * command = args[i++];
                char * new_args[16] = {0};
                int newi = 0;
                while(args[i][0] != '|') {
                    new_args[newi++] = args[i++];
                }
                newi++;
                execvp(command, new_args);
                exit(0);
            }
        }

        pid_t pid = fork();

        for(int i = 0; i < pidi; i++) {
            int status;
            waitpid(pids[pidi], &status, 0);

            if(status != 0) {
                printf("Error at: %s\n", command);
                exit(1);
            }
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