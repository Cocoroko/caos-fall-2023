//
// Created by Eseniya Lushko on 18.10.2023.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

int main() {
    pid_t pid;
    pid = fork();
    int counter = 1;
    while (1) {
        if (pid == 0) {
            pid = fork();
            ++counter;
            if (pid == -1) {
                printf("%d", counter);
                return 0;
            }
        } else {
            sched_yield();
        }
    }
}

