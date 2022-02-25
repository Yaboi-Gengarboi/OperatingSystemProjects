// OS Lab2 Processes
// sampleProgram2Windows.c
// Created on 2022-02-03 by Justyn Durnford
// Last modified on 2022-02-03 by Justyn Durnford

#ifdef WIN32

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, wchar_t* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (argc != 2)
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process. 
    if (CreateProcessW(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("CreateProcess succeeded.\n");
    }
    else
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

#endif // #ifdef WIN32