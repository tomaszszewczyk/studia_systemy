#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>

bool waiting = false;

void signal_handler(int signal) {

    if(waiting) {
        waiting = false;
    } else {
        printf("Oczekuje na CTRL+Z - kontynuacja albo CTR+C - zakonczenie programu\n");
        waiting = true;
    }
}

void int_handler(int signal) {
    printf("Odebrano sygnał SIGINT\n");
    exit(0);
}

int main ()
{
    time_t rawtime;
    struct tm * timeinfo;

    struct sigaction new_action;
    new_action.sa_handler = int_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction (SIGINT, &new_action, NULL);
    signal(SIGTSTP, signal_handler);

    while(true) {
        if(waiting)
            continue;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf ( "Current local time and date: %s", asctime (timeinfo) );
        sleep(1);
    }

    return 0;
}