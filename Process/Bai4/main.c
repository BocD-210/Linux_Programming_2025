#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    pid_t pid;
    pid = fork();

    if(pid < 0){
        printf("Fork failed\n");
    }
    else if(pid == 0){          //child process
        int value = atoi(argv[1]);
        printf("Child process is running, my PID is: %d\n", getpid());
        //while(1); // See result after run while(1) and "kill -9 ..." in terminal
        sleep(2);
        exit(value);
    }
    else{         
        int status;              //parent process
        int retVal = wait(&status);
        if(retVal == -1){
            printf("wait() failed\n");
            return 0;
        }
        if(WIFEXITED(status)){
            printf("Child process exited normally termination w value is: %d, C.PID is: %d\n", WEXITSTATUS(status), retVal);
            printf("Value status: %d\n", status); // khong hieu gia tri status nay la gi` @@
        }
        else{
            printf("Child process exited abnormally termination, C.PID is %d\n", retVal); // WEXITSTATUS() dont have return value
        }
    }
}