#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>


static void* func(void* arg) {
    pid_t pid = getpid();
    int64_t result = 0;
    int32_t val;
    printf("new thread \n");
    if (scanf("%d ", &val) != -1) {
        result += val;
        printf("in current thread scanned number = %d \n", val);
    }
    printf("new thread \n");
    return (void*)result;
}

int main(int argc, char* argv[]) {
    uint64_t N = strtol(argv[1], NULL, 10);
    uint64_t result  = 0;

    pthread_t* threads = calloc(N, sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN / 4);
    pthread_attr_setguardsize(&attr, 0);

    for(int64_t index = 0; index < N; ++index) {
        pthread_create(&threads[index], &attr, func, 0);
    }


    for(int64_t index = 0; index < N; ++index) {
        void* counter;
        pthread_join(threads[index], &counter);
        result += (uint64_t)counter;
    }

    printf("%ld \n", result);
    pthread_attr_destroy(&attr);
    return 0;
}
