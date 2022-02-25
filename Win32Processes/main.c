// Win32Processes
// main.c
// Created on 2022-02-09 by Justyn Durnford
// Last modified on 2022-02-10 by Justyn Durnford
// 

#ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

int main(int argc, wchar_t* argv[])
{
    printf("Parent Process created.\n");
    printf("Parent Process ID: %u\n", GetCurrentProcessId());

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    unsigned long pid = CreateProcessW(L"E:\\OperatingSystemProjects Repository\\Win32ProcessesChildProgram\\Release\\Win32ProcessesChildProgram.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    // Start the child process. 
    if (pid != 0)
    {
        printf("CreateProcess succeeded.\n");
    }
    else
    {
        printf("CreateProcess failed (%u).\n", GetLastError());
        return;
    }

    // printf("Process ID: %d\n", pi.dwProcessId);

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Parent Process is now terminating.\n");
}

#endif // #ifdef WIN32