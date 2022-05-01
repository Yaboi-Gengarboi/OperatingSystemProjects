// Win32Processes
// main.c
// Created on 2022-02-09 by Justyn Durnford
// Last modified on 2022-04-11 by Justyn Durnford
// 

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <windows.h>

class Win32Process
{
    private:

    int _pid;
    string _path;

    public:

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    private:

    // 
    void _start()
    {
        _pid = CreateProcessA(_path.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

        if (_pid == 0)
            cout << "CreateProcess failed: " << GetLastError() << endl;
    }

    // 
    void _close() noexcept
    {
        if (pi.hProcess)
        {
            CloseHandle(pi.hProcess);
            pi.hProcess = nullptr;
        }
        if (pi.hThread)
        {
            CloseHandle(pi.hThread);
            pi.hThread = nullptr;
        }
    }

    public:

    // Constructor.
    Win32Process(const string& exe_path)
    {
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        _pid = 0;
        _path = exe_path;
    }

    // Destructor.
    ~Win32Process() noexcept
    {
        _close();
    }

    // 
    int start()
    {
        _start();
        return _pid;
    }

    // 
    void close() noexcept
    {
        _close();
    }

    // 
    int getID() const noexcept
    {
        return _pid;
    }

    // 
    HANDLE processHandle() noexcept
    {
        return pi.hProcess;
    }
};

int main(int argc, char** argv)
{
    cout << "Process 1 created." << endl;
    cout << "Process 1 ID: " << GetCurrentProcessId() << endl;

    Win32Process proc("E:\\OperatingSystemProjects Repository\\Win32ProcessesChildProgram\\Release\\Win32ProcessesChildProgram.exe");
    if (proc.start())
    {
        cout << "CreateProcess succeeded." << endl;
    }

    // Wait until Process 2 exits.
    WaitForSingleObject(proc.processHandle(), INFINITE);
    proc.close();

    cout << "Process 1 is now terminating." << endl;
}