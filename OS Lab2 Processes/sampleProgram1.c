// OS Lab2 Processes
// sampleProgram1.c
// Obtained from https://alaska.instructure.com/courses/6364/assignments/115215 on 2022-01-31
// Last modified on 2022-01-31 by Justyn Durnford

#if defined(__unix__)

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigHandler(int sigNum)
{
    printf(" received an interrupt signal through Ctrl-C.\n");
    printf("Going to sleep.\n Notice the process is not exiting immediately");

    // this is where shutdown code would be inserted
    sleep(1);
    printf("Out of here.\n");
    printf("Now the process will terminate with the exit system call \n");
    exit(0);
}

int main()
{
    signal(SIGINT, sigHandler);
    printf("waiting...\n");
    pause();
    return 0;
}

#endif // #if defined(__unix__)