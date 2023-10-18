#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

int main() {
    pid_t pid;
    do {
        pid = fork();
    } while (-1 != pid);

    printf("Process %d reached out limit on processes\n", getpid());
    while (1) {
        sched_yield();
    }
}