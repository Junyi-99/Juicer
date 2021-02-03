#include <stdio.h>
#include <unistd.h>

int main() {
    unsigned long long counter = 0;
    while (1) {
        printf("%llu\n", counter++);
        usleep(500 * 1000); // 500 ms
        if (counter < 0)
            break;
    }
    return 0;
}