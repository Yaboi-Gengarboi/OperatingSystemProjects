// OS Lab2 Processes
// sampleProgram1Unix.c
// Obtained from https://alaska.instructure.com/courses/6364/assignments/115215 on 2022-01-31
// Last modified on 2022-02-03 by Justyn Durnford

// This program is intended to be run the Unix terminal.
// Find the location of "OSLab2Processes.out" and run the terminal
// in the same directory.
// Type: ./OSLab2Processes.out and hit enter.
// Press CTRL and C on the keyboard and see the results!

#if defined(__unix__)

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigHandler(int sigNum)
{
    printf("Received an interrupt signal.\n");
    printf("Going to sleep.\nNotice the process is not exiting immediately.\n");

    // This is where shutdown code would be inserted.
    sleep(1);
    printf("Now the process will terminate with the exit system call.\n");
    exit(0);
}

int main()
{
    signal(SIGINT, sigHandler);
    printf("Waiting...\n");
    pause();
    return 0;
}

#endif // #if defined(__unix__)