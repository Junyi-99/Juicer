#include <stdio.h>

int foo(int n){
    printf("I'm level %d\n", n);
    foo(++n);
    return 0;
}

int main() {
    foo(1);
    return 0;
}