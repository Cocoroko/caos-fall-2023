#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char buff[4096];
    int counter = 0;
    int status = 0;
    int exit = 0;
    setbuf(stdin, NULL);
    while (!exit) {
        pid_t pid = fork();
        if (pid < 0) {
            return 1;
        }
        else if (pid == 0) {
            return scanf("%s", buff) == EOF;
        }
        else {
            waitpid(pid, &status, 0);
            exit = WEXITSTATUS(status);
            if (!exit)
                ++counter;
        }
    }
    printf("%d \n", counter);
    return 0;
}
