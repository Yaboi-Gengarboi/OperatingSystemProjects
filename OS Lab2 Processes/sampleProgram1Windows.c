// OS Lab2 Processes
// sampleProgram1Windows.c
// Created on 2022-01-31 by Justyn Durnford
// Last modified on 2022-02-03 by Justyn Durnford

// This program is intended to be run using Windows Powershell.
// Find the location of "OS Lab2 Processes.exe" and run Windows Powershell
// in the same directory.
// Type: .\"OS Lab2 Processes.exe" and hit enter.
// Press CTRL and C on the keyboard and see the results!

#ifdef WIN32

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void sigHandler(int sigNum)
{
    printf("Received an interrupt signal.\n");
    printf("Going to sleep.\nNotice the process is not exiting immediately.\n");

    // This is where shutdown code would be inserted.
    Sleep(1000);
    printf("Now the process will terminate with the exit system call.\n");
    exit(0);
}

int main()
{
    signal(SIGINT, sigHandler);
    printf("Waiting...\n");

    // Infinite loop.
    while (1 == 1)
    {

    }

    return 0;
}

#endif // #ifdef WIN32