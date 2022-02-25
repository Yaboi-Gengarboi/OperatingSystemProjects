// Win32ProcessesChildProgram
// main.c
// Created on 2022-02-10 by Justyn Durnford
// Last modified on 2022-02-10 by Justyn Durnford
// 

#ifdef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

int main(int argc, wchar_t* argv[])
{
	printf("Child Process created.\n");
	printf("Child Process ID: %u\n", GetCurrentProcessId());
	printf("Going to sleep for 5 seconds\n");
	Sleep(5000);
	printf("Child Process is now terminating.\n");

	return 0;
}

#endif // #ifdef WIN32