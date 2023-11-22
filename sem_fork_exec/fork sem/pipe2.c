//
// Created by Eseniya Lushko on 18.10.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char * argv[]) {
    char* cmd1 = argv[1];
    char* cmd2 = argv[2];
    int pipefd[2];
    if(pipe(pipefd) < 0) {
        perror("pipe error");
        exit(1);
    }
    pid_t pid = fork();
    if(pid == 0) {
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        if(execlp(cmd1, cmd1, NULL) == -1) {
            perror("exec error");
            exit(1);
        }
    }
    else {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        waitpid(pid, 0, 0);
        if(execlp(cmd2, cmd2, NULL) == -1) {
            perror("exec error");
            exit(1);
        }
    }
    return 0;
}
