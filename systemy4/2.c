#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

bool ph1_received = false;
bool ph2_received = false;

int  sent     = 0;
int  received = 0;
int  cpid;

void ph1(int signal) {
    received++;
    ph1_received = true;
}

void ph2(int signal) {
    received++;
    ph2_received = true;
}

void ih(int signal) {
    kill(cpid, SIGINT);
    exit(0);
}

void parent(int pid, int l, int type) {
    signal(SIGUSR1, ph1);
    signal(SIGUSR2, ph2);
    cpid = pid;

    if(type == 1) {
        for(int i = 0; i < l; i++) {
            kill(pid, SIGUSR1);
            sent++;
        }
        kill(pid, SIGUSR2);
        sent++;
    } else if(type == 2) {
        for(int i = 0; i < l; i++) {
            kill(pid, SIGUSR1);
            sent++;
            while(!ph1_received) {
                pause();
            }
            ph1_received = false;
        }
        kill(pid, SIGUSR2);
        sent++;
    } else if(type == 3) {
        kill(pid, SIGABRT);
        sent++;
        kill(pid, SIGALRM);
        sent++;
    } else {
        printf("Wrong type\n");
    }

    waitpid(pid, NULL, 0);
    printf("Sent %d\n", sent);
    printf("Received %d\n", received);
}


void ch1(int signal) {
    received++;
    kill(getppid(), SIGUSR1);
}

void ch2(int signal) {
    received++;
    printf("Received from parent %d\n", received);
    exit(0);
}

void child() {
    signal(SIGUSR1, ch1);
    signal(SIGUSR2, ch2);

    while(true) {
        pause();
    }
}

void main(int argc, char** argv) {
    if(argc != 3) {
        printf("Wrong number of args\n");
    }

    int l    = atoi(argv[1]);
    int type = atoi(argv[2]);
    int pid  = fork();

    if(pid == 0) {
        child();
    } else {
        parent(pid, l, type);
    }
}