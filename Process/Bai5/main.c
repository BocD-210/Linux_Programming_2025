#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void waitHandler(int signum){
    printf("waitHandler() is running\n");
    wait(NULL);
}
int main(){
    pid_t pid;
    pid = fork();

    if(pid == 0){
        printf("Im child process, my pid is: %d\n", getpid());
        sleep(2);
        printf("Child exited\n");
    }
    else{
        signal(SIGCHLD, waitHandler);
        printf("Im parent\n");
        while(1);
    }
}