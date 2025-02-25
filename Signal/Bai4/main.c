#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

void sig_handler(int num){
    printf("\nSIGTSTP ignored\n");
    //exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGTSTP, sig_handler);
    if(signal(SIGTSTP, sig_handler) == SIG_ERR){
        fprintf(stderr, "cannot handler SIGTSTP\n");
        exit(1);
    }
    while(1);
}