#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>


volatile sig_atomic_t signal_ = 0;

void SIGALARMHandler() {
    signal_ = 1;
}

void SIGCHLDHandler() {
    signal_ = 2;
}

int main(int argc, char* argv[]) {
    size_t time = strtol(argv[1], NULL, 10);

    char str1[] = "timeout";
    char str2[] = "signaled";
    char str3[] = "ok";

    struct sigaction sigalarm_handler;
    memset(&sigalarm_handler, 0, sizeof(sigalarm_handler));
    sigalarm_handler.sa_handler = SIGALARMHandler;
    sigalarm_handler.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sigalarm_handler, NULL);

    struct sigaction sigchild_handler;
    memset(&sigchild_handler, 0, sizeof(sigchild_handler));
    sigchild_handler.sa_handler = SIGCHLDHandler;
    sigchild_handler.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sigchild_handler, NULL);


    pid_t pid = fork();
    alarm(time);

    if (!pid) {
        execvp(argv[2], argv + 2);
    } else {
        while (!signal_) {}

        if (signal_ == 1) {
            kill(pid, SIGKILL);
            printf("%s", str1);
            return 2;
        }

        if (signal_ == 2) {
            int status;
            int q = WIFSIGNALED(status);
            if (q) {
                printf("%s", str2);
                return 1;
            } else {
                printf("%s", str3);
                return 0;
            }
        }
    }
}
