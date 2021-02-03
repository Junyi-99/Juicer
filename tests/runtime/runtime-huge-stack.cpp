#include <stdio.h>

int main() {
    int a[1024 * 1024 * 100] = {0}; // 800 MB
    printf("runtime-huge-stack: %d\n", 0)
    return 0;
}