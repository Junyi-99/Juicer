#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int a = 0;
    printf("%d", 5 / a);
    return EXIT_SUCCESS;
}