#include <stdio.h>
#include <stdlib.h>

#define STEP 4096 // Bytes
// getconf PAGE_SIZE
int main() {
    double counter = 0;
    void *ptr;
    while (1) {
        ptr = malloc(STEP);
        if (ptr == NULL) {
            printf("malloc failed until %f MB\n", (counter / 1048576.0));
            return EXIT_FAILURE;
        } else {
            counter += STEP;
            printf("%f MB\n", (counter / 1048576.0));
        }
    }
    return EXIT_SUCCESS;
}