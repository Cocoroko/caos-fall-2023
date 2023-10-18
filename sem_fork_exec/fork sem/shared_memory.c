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
    int shared_memory = 2;
    pid = fork();
    if (pid == 0) {
        printf("in child shared_memory = %d \n", shared_memory);
        shared_memory += 4;
        _exit(EXIT_FAILURE);
    } else {
        int status;
        int* pointer = &status;
        waitpid(-1, pointer, 0);
        if (!WIFEXITED(pointer)) {
            printf("child did not exit normally \n");
        } else {
            printf("child exit normally \n");
        }
        printf("in parent shared_memory = %d \n", shared_memory);
    }
}
