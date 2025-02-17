#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>

using namespace std;

LPCWSTR PIPE_NAME = L"ServiceLog.txt";
LPCWSTR childprocess = L"C:\\Users\\Tertan41k\\Desktop\\Process 3\\servicedthreads\\x64\\Debug\\servicedthreads.exe";

int main() {
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
    STARTUPINFO si = { sizeof(si) }; // Инициализация структуры STARTUPINFO
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;

    HANDLE hPipe = CreateFile(
        PIPE_NAME,
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("CreateFile failed.\n");
        return 1;
    }

    // Создание дочернего процесса
    if (!CreateProcess(
        childprocess,
        NULL,
        &sa,
        NULL,
        TRUE,
        CREATE_NEW_CONSOLE, // Создание новой консоли
        NULL,
        NULL,
        &si,
        &pi)) {
        cout << "Не удалось создать дочерний процесс. Ошибка: " << GetLastError() << "\n";
        CloseHandle(hPipe);
        return 0;
    }

    cout << "Press any key";
    _getch();

    char command;
    cout << "Select an option: \n";
    cout << "M-create new Math thread \nO-create new Output thread\n[0-9]-terminate thread with this number\nQ-exit\n";

    while (scanf_s(" %c", &command) == 1) {
        DWORD bytesWritten;
        if (!WriteFile(hPipe, &command, sizeof(command), &bytesWritten, NULL)) {
            cout << "WriteFile failed.\n";
            break;
        }

        if (command == 'M') {
            cout << "Command sent: Create Math thread.\n";
        }
        else if (command == 'O') {
            cout << "Command sent: Create Output thread.\n";
        }
        else if (isdigit(command)) {
            cout << "Command sent: Terminate thread " << command << ".\n";
            char buffer[64];
            DWORD bytesRead;
            if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
                buffer[bytesRead] = '\0';
                cout << buffer;
            }
            else {
                cout << "ReadFile failed.\n";
            }
        }
        else if (command == 'Q') {
            cout << "Command sent: Quit.\n";
            break;
        }
        else {
            cout << "Unknown command: " << command << "\n";
        }

        cout << "Select an option: \n";
        cout << "M-create new Math thread \nO-create new Output thread\n[0-9]-terminate thread with this number\nQ-exit\n";
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(hPipe);
    return 0;
}
