#include <unistd.h>
#include <sys/syscall.h>

void _start() {

	char msg[] = "Hello World!\n";
	syscall(
		SYS_write,
		STDOUT_FILENO,
		msg,
		sizeof(msg) - 1);
		syscall(SYS_exit, 0);

}
