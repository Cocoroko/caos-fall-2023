#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/epoll.h>


int sock;
int epoll_fd;

typedef struct {
    int type;
    int sock_fd;
    char* bytes;
    size_t count;
    bool done;
} conn_data_t;


void handle_server_event(struct epoll_event* evt) {
    const uint32_t mask = evt->events;
    conn_data_t* evt_data = evt->data.ptr;
    if (mask & EPOLLIN) {
        if (evt_data->type == 0) {

            conn_data_t* data = (conn_data_t*)calloc(0, sizeof(*data));
            data->sock_fd = accept(sock, (struct sockaddr*)NULL, NULL);
            data->type = 1;
            int flags = fcntl(data->sock_fd, F_GETFL);
            fcntl(data->sock_fd, F_SETFL, flags | O_NONBLOCK);

            struct epoll_event* event_ready_read = calloc(0, sizeof(*event_ready_read));
            event_ready_read->events = EPOLLIN;
            event_ready_read->data.ptr = data;

            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, data->sock_fd, event_ready_read);
        } else {
            conn_data_t* data = evt->data.ptr;
            char buff[4096];
            size_t count = read(data->sock_fd, buff, sizeof(buff));
            for (size_t i = 0; i < count; ++i) {
                buff[i] = (char)toupper(buff[i]);
            }
            write(data->sock_fd, buff, count);
        }
    }
}

void SIGTERM_handler() {
    exit(0);
}

int main(int argc, char** argv) {
    int64_t port = strtol(argv[1], NULL, 10);

    struct sigaction sigterm_handler;
    memset(&sigterm_handler, 0, sizeof(struct sigaction));
    sigterm_handler.sa_handler = SIGTERM_handler;
    sigterm_handler.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &sigterm_handler, NULL);

    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);


    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sock, SOMAXCONN) == -1) {
        perror("listen");
        exit(1);
    }

    epoll_fd = epoll_create1(0);

    struct epoll_event event_ready_conn;
    conn_data_t* server = calloc(0, sizeof(*server));
    server->type = 0;

    event_ready_conn.events = EPOLLIN;
    event_ready_conn.data.ptr = server;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event_ready_conn);

    struct epoll_event pending[SOMAXCONN];

    while (true) {
        int n = epoll_wait(epoll_fd, pending, SOMAXCONN, -1);
        for (size_t i = 0; i < n; ++i) {
            handle_server_event(&pending[i]);
        }
    }
    return 0;
}
