#include <unistd.h>


int main() {

	char msg[] = "Hello world!\n";
	write(STDOUT_FILENO, msg, sizeof(msg) - 1);
	return 0;

}
