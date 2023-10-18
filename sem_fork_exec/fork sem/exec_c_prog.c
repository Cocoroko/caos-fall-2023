#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

enum { BUFFSIZE = 5000, CODESIZEDIFF = 100 };

const char PROG_CODE[] = "#include <stdio.h>\n"
                         "int main() {\n"
                         "int x = 4; \n"
                         "printf(\"Hello guys  %d\", x);\n}";

int main() {
    char buffer[BUFFSIZE - CODESIZEDIFF] = {};
    fgets(buffer, BUFFSIZE, stdin);
    int length = strlen(buffer);
    if (buffer[length - 1] == '\n')
        buffer[length - 1] = 0;
    char c_code[BUFFSIZE] = {};
    snprintf(c_code, BUFFSIZE, PROG_CODE, buffer);
    int fd = open("StrangeFile.c", O_CREAT | O_RDWR, 0777);
    int res = 0;
    while (res < strlen(c_code)) {
        res = write(fd, c_code, strlen(c_code));
    }
    close(fd);

    int info;

    pid_t pid = fork();
    if (pid == -1)
        return 1;
    else if (pid == 0) {
        execlp("gcc", "gcc", "StrangeFile.c", "-o", "StrangeFile", NULL);
        return 2;
    } else {
        waitpid(pid, &info, 0);
    }
    pid = fork();
    if (pid == -1)
        return 1;
    else if (pid == 0) {
        execlp("./StrangeFile", "./StrangeFile", NULL);
        return 3;
    } else {
        waitpid(pid, &info, 0);
    }
    unlink("StrangeFile.c");
    unlink("StrangeFile");
}
