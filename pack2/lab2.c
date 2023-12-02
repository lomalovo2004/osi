#include <stdio.h>
#include <unistd.h>

int main(){
    int pid;
    pid = fork();
    printf("%d\n", pid);
    fork();
    printf("Hi\n");
    fork();
    printf("Hi\n");

}

