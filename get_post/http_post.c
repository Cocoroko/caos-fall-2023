#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char *argv[]) {
    char *host_name = argv[1];
    char *path = argv[2];
    char *file_path =argv[3];

    signal(SIGPIPE, SIG_IGN);

    struct addrinfo addr_hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_protocol = 0 };
    struct addrinfo *addr_result = NULL;

    getaddrinfo(host_name, "http", &addr_hints, &addr_result);
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (0!=connect(sock, addr_result->ai_addr, addr_result->ai_addrlen)) {
        perror("connect");
        exit(1);
    }

    char minibuf[66666];
    int headers_completed = 0;

    FILE* external_file = fopen(file_path, "r");
    fseek(external_file, 0, SEEK_END);
    size_t file_size = ftell(external_file);
    fseek(external_file, 0, SEEK_SET);

    char * contents = (char*)malloc(file_size+1);
    memset(contents, 0, file_size+1);
    fread(contents, 1, file_size, external_file);

    char* request = (char*)malloc(file_size+2048);
    snprintf(request,
             file_size+2048,
             "POST %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: multipart/form-data\r\n"
             "Connection: close\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s\r\n"
             "\r\n",
             path, host_name, file_size, contents);

    write(sock, request, strnlen(request,file_size+4096));

    FILE* in = fdopen(sock, "r");
    while (fgets(minibuf, sizeof(minibuf), in)) {
        if (strcmp(minibuf, "\n") == 0 || strcmp(minibuf,"\r\n") == 0) {
            headers_completed = 1;
            continue;
        }
        if (headers_completed) {
            printf("%s", minibuf);
        }
    };

    close(sock);
    fclose(in);
    free(request);
    free(contents);
    fclose(external_file);
    return 0;
}
