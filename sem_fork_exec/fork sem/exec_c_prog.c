#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

enum { BUFFSIZE = 5000, CODESIZEDIFF = 100 };

const char PROG_CODE[] = "#include <stdio.h>\n"
                         "int main() {\n"
                         "printf(\"Hello guys\");\n}";

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
    // while (res < strlen(c_code)) {
        res = write(fd, c_code, strlen(c_code));
    // }
    close(fd);

    int info;

    pid_t pid = fork();
    if (pid == -1)
        return 1;
    else if (pid == 0) {
        execlp("gcc", "gcc", "StrangeFile.c", "-o", "StrangeFile", NULL);
        printf("hello from compiled program \n");
        return 2;
    } else {
        printf("in parent!\n");
        waitpid(pid, &info, 0);
        execlp("./StrangeFile", "./StrangeFile", NULL);
        printf("dgfghjghfgf");
    }



}
