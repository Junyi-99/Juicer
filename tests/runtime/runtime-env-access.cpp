#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("%s\n", getenv("PATH"));
    printf("%s\n", getenv("PWD"));
    return 0;
}