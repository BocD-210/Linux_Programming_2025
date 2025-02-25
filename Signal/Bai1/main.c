#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int sig_cnt = 0;

void sig_handler(int num)
{
    printf("\nSIGINT received\n");
    ++sig_cnt;
    if (sig_cnt == 3)
    {
        exit(EXIT_SUCCESS);
    }
}

int main()
{
    signal(SIGINT, sig_handler);
    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
        fprintf(stderr, "Cannot handle SIGINT\n");
        exit(EXIT_FAILURE);     
    }
    while (1);
    return 0;
}
