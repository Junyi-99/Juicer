#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    while (1) {
        pid_t pid = fork();
        if (pid < 0) {
            //perror("fork failed");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            printf("child pid = %d, parent pid = %d, pgrp = %d, pgid = %d\n", getpid(), getppid(), getpgrp(), getpgid(getpid()));
            continue;
        } else {
            int stat = 0;
            printf("pid = %d\n", pid);
            //wait(&stat);
        }
    }

    return EXIT_SUCCESS;
}