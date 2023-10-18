#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

int starting_num;
int sv[2];
bool is_first = true;
bool finish_process = false;

static void *func1(void* arg) {
    int N;
    while (true) {
        if (finish_process) {
            return NULL;
        }
        if (is_first) {
            is_first = false;
            N = starting_num;
        } else {
            read(sv[1], &N, sizeof(int));
        }
        N -= 3;
        if (N == 0 || N > 100) {
            printf("%d\n", N);
            finish_process = true;
            if (finish_process) {
                write(sv[1], &N, sizeof(int));
                return NULL;
            }
        } else {
            if (finish_process) {
                return NULL;
            }
            printf("%d\n", N);
            write(sv[1], &N, sizeof(int));
        }
    }


}

static void *func2(void* arg) {
    int N;
    while (true) {
        if (finish_process) {
            return NULL;
        }
        read(sv[0], &N, sizeof(int));
        N += 5;
        if (N == 0 || N > 100) {
            printf("%d\n", N);
            finish_process = true;
            if (finish_process) {
                write(sv[0], &N, sizeof(int));
                return NULL;
            }
        } else {
            if (finish_process) {
                return NULL;
            }
            printf("%d\n", N);
            write(sv[0], &N, sizeof(int));
        }
    }
}

int main(int argc, char *argv[]) {
    uint64_t starting_number = strtol(argv[1], NULL, 10);
    starting_num = starting_number;
    socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, func1, 0);
    pthread_create(&threads[1], NULL, func2, 0);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    return 0;

}