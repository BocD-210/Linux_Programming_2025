#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    pid_t pid;
    printf("Main process is running\n");
    sleep(2);
    if(argc != 2){
        printf("Check number of argc !!!\n");
        return 0;
    }

    pid = fork();

    if(pid < 0){
        printf("Fork failed !\n");
    }
    if(pid == 0){
        printf("Main stop !\n"); // khi 

        char *cd = argv[1];
        if(strcmp(argv[1], "1") == 0){
            char *const args[] = {"ls", "-l", "-a", NULL};
            //execlp("/usr/bin/ls", "ls", "-l", "-a", NULL);
            execvp("ls", args); // chu y cach setup cua cac EXEC family (Moi loai moi cach setup)
        }
        else if(strcmp(argv[1], "2") == 0){
            execlp("/usr/bin/date", "date", NULL);
        }
        else{
            printf("Command line argument is invalid\n");
            exit(1); // END child process with wrong command line
        }
    }
    else{
        wait(NULL);
        printf("Child process completed, main process will continue\n");
    }
}