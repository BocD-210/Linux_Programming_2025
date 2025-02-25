#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]){
    pid_t  pid = fork();   // fork return value of child process;
    int counter = 2;

    printf("gia tri khoi tao cua counter: %d\n", counter);
    // se in ra 2 dong tren tai vi` sau khi goi fork() ca 2 tien trinh cha & con deu thuc hien cac len o duoi 1 cac doc lap
    if(0 == pid){   // Children process
        printf("Child PID is: %d, Parent PID is: %d\n", getpid(), getppid());
    }
    else if(pid > 0){           // Parent process
        printf("My child PID is: %d, My PID is: %d\n", pid, getpid());
    }
    else
        printf("Fork failed");

    return 0;
}