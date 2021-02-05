#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    while (1) {
        pid_t pid = fork();
        if (pid < 0) {
            return EXIT_FAILURE;
        } else if (pid == 0) {
            printf("child pid = %d, parent pid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));
            continue;
        } else {
            printf("pid = %d\n", pid);
        }
    }

    return EXIT_SUCCESS;
}