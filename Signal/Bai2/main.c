#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int cnt = 1;

void sig_handler(int num)
{
    printf("Timer: %d seconds !\n", cnt);
    cnt++;

    if (cnt <= 10)
    {
        alarm(1);
    }
    else
    {
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    if (signal(SIGALRM, sig_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handler SIGALRM\n");
        exit(EXIT_FAILURE);
    }

    alarm(1);
    while (1)
    {
        pause();
    }
}