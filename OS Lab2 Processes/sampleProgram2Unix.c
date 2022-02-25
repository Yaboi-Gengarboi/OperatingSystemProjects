// OS Lab2 Processes
// sampleProgram2Unix.c
// Created on 2022-02-03 by Justyn Durnford
// Last modified on 2022-02-08 by Justyn Durnford
// Compile with g++ sampleProgram2Unix.c -o sampleProgram2Unix.out

#if defined(__unix__)

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void sigHandler(int sig_num)
{
    if (sig_num == SIGUSR1)
        printf("Received signal SIGUSR1\n");
    if (sig_num == SIGUSR2)
        printf("Received signal SIGUSR2\n");
    if (sig_num == SIGINT)
        exit(0);
}

int main()
{
    int status = 0;

    signal(SIGUSR1, sigHandler);
    signal(SIGUSR2, sigHandler);
    signal(SIGINT, sigHandler);

    pid_t pid = fork();

    if (pid == 0) // Child process
    {
        pid_t ppid = getppid();
        srand(time(0));
	    int dur;
        int sig;

	    while (1)
	    {
	        dur = rand() % 5;
	        sig = rand() % 2;
	        sleep(dur);

	        if (sig)
		        kill(ppid, SIGUSR1);
	        else
		        kill(ppid, SIGUSR2);
	    }
    }
    else // Parent process
    {
	    waitpid(pid, &status, 0);
    }

    return 0;
}

#endif // #if defined(__unix__)