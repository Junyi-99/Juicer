#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    system("shutdown -r now");
    return EXIT_SUCCESS;
}