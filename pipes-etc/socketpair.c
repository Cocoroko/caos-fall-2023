#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


// cat "hello" | wc -l 
//
int main() {
    int sv[2];
    int result = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    if (result < 0) {
        exit(1);
    }
    printf("sv[0] is : %d \n", sv[0]);
    printf("sv[1] is : %d \n", sv[1]);

    if (fork()) {
        int val = 0;
        pid_t pid = getpid();

        close(sv[1]);   
        while (1) {
            ++val;
            printf("%d proc send message: %d\n", pid, val);
            write(sv[0], &val, sizeof(val));
                        
            sleep(1);
            read(sv[0], &val, sizeof(val));
            // printf("%d proc receive message %d\n", pid, val);
        }
    } else {
        int val = 0;
        close(sv[0]);
        pid_t pid = getpid();
        while (1) {
            read(sv[1], &val, sizeof(val));
            printf("%d proc receive message: %d\n", pid, val);
            // printf("%d procc send message %d\n", pid, val);
            // write(sv[1], &val, sizeof(val));
        }
    }
}
