// Win32ProcessesChildProgram
// main.cpp
// Created on 2022-02-10 by Justyn Durnford
// Last modified on 2022-04-11 by Justyn Durnford
// 

#include <iostream>
using std::cout;
using std::endl;

#include <windows.h>

int main(int argc, char** argv)
{
	cout << "Process 2 created." << endl;
	cout << "Process 2 ID: " << GetCurrentProcessId() << endl;
	cout << "Going to sleep for 5 seconds." << endl;
	Sleep(5000);
	cout << "Process 2 is now terminating." << endl;

	return 0;
}