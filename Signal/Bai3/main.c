#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

int cnt = 0;

void sig_handler(int num)
{
    ++cnt;
    printf("Received SIGNAL NUM %d from parent: %d\n", num, cnt);
    if (cnt >= 5)
        exit(EXIT_SUCCESS);
}

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Fork() failed\n");
        exit(0);
    }
    else if (pid == 0) // Children process
    {
        printf("Child PID is: %d\n", getpid());
        signal(SIGUSR1, sig_handler);

        while (1)
        {
            pause();
        }
    }
    else
    {
        sleep(1);

        printf("Parent process sending signal to child (PID: %d)\n", pid);
        for (int i = 1; i <= 5; i++)
        {
            printf("Sending signal to child process %d\n", i);
            kill(pid, SIGUSR1);
            sleep(1);
        }
        while(1);
    }
}
