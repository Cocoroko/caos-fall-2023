// Программа в аргументах командной строки получает неотрицательное число
// (разрядность заранее не определена).
// Требуется вывести на экран значение квадрата этого числа.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
    int fd = open("my.sh", O_WRONLY | O_TRUNC | O_CREAT, 0700);
    FILE* f = fdopen(fd, "w");
    fprintf(f,
            "#! /bin/bash\n"
            "echo Hi > my_file.txt\n",
            argv[1]);
    fclose(f);
    int status = execl("my.sh", "my.sh", NULL);
    return status;
}