#include <cstdio>
#include <sys/signal.h>
#include <unistd.h>

int main(){
    alarm(1);
    raise(SIGSEGV);
    sleep(2);
    printf("alarm exit\n");
    return 0;
}