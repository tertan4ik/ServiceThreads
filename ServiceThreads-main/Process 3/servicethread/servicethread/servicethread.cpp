#include <iostream>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string>
using namespace std;

LPCWSTR Child_process = L"C:\\Users\\st310-06\\Desktop\\ServiceThreads-main\\Process 3\\servicedthreads\\x64\\Debug\\servicedthreads.exe";

int main() {
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
	CRITICAL_SECTION cs;
    STARTUPINFO si = { sizeof(si) }; // Инициализация структуры STARTUPINFO
	sa.lpSecurityDescriptor = NULL;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	HANDLE hPipe = CreateNamedPipe(
		L"\\\\.\\pipe\\MyPipe",
		PIPE_ACCESS_DUPLEX, // Двусторонний доступ
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		512, 512, 0, &sa);

	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("CreateNamedPipe failed.\n");
		GetLastError;
		_getch();
		return 1;
	}


    // Создание дочернего процесса
    if (!CreateProcess(
        Child_process,
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
	_getch;
    cout << "Press any key";
    _getch();

    char command;
    cout << "Select an option: \n";
    cout << "M-create new Math thread \nO-create new Output thread\n[0-9]-terminate thread with this number\nQ-exit\n";

    while (scanf_s(" %c", &command)==1) {
	
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
        }
        else if (command == 'Q') {
            cout << "Command sent: Quit.\n";
            break;
        }
        else {
            cout << "Unknown command: " << command << "\n";
        }
		LeaveCriticalSection(&cs);
        cout << "Select an option: \n";
        cout << "M-create new Math thread \nO-create new Output thread\n[0-9]-terminate thread with this number\nQ-exit\n";
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(hPipe);
    return 0;
}
