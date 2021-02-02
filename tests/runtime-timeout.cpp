#include <stdio.h>
#include <unistd.h>
int main() {
    unsigned long long counter = 0;
    while (1) {
        printf("%llu\n", counter++);
        sleep(1);
        if (counter < 0)
            break;
    }
    return 0;
}