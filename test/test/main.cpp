#include <iostream>
#include <windows.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi; // The function returns this
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CONST wchar_t* commandLine = TEXT("1 2 23");
    
    for (int i = 0 ; i < 30; i++)
        if (!CreateProcessW(
            L"C:\\Users\\sasha\\Documents\\VS\\task1\\Client\\Release\\Client.exe",
            (LPTSTR)commandLine,    
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory
            &si,            // Pointer to STARTUPINFO structure
            &pi)           // Pointer to PROCESS_INFORMATION structure
            )
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            throw std::exception("Could not create child process");
        }
        else
        {
        }
}