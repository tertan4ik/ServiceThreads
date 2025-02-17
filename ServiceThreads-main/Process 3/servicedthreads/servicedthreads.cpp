#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;

#define MAX_THREADS 10
bool WORK = true;
bool ALLERT = false;

DWORD WINAPI ThreadMath(LPVOID idParams) {
	int index = (int)idParams;
	cout << "THREAD-" << index << "-MATH" << "-STARTED\n";
	double x = 0;
	for (size_t i = 0; i < 20; i++) {
		x += x + i * 1.5;
		cout << "Thread " << "[" << index << "]" << "(MATH)-" << x << "\n";

		if (WORK == false) {
			break;
		}
		while (ALLERT) {
			Sleep(100);
		}
		Sleep(1000);
	}
	cout << "THREAD-" << index << "-MATH" << "-ENDED\n";
	return 0;
}

DWORD WINAPI ThreadOutput(LPVOID idParams) {
	int index = (int)idParams;
	cout << "THREAD-" << index << "-OUTPUT" << "-STARTED\n";

	for (size_t i = 0; i < 20; i++) {
		cout << "Thread " << "[" << index << "]" << "(OUTPUT)-" << "SIMULATION OF DATA OUTPUT\n";

		if (WORK == false) {
			break;
		}
		while (ALLERT) {
			Sleep(100);
		}
		Sleep(1000);
	}
	cout << "THREAD-" << index << "-OUTPUT" << "-ENDED\n";
	return 0;
}

int main() {

	cout << "process created";
	_getch;
	HANDLE hThreads[MAX_THREADS] = { NULL };
	DWORD IDThread[MAX_THREADS] = { NULL };




	HANDLE hPipe = CreateFile(
		L"\\\\.\\pipe\\MyPipe",
		GENERIC_READ | GENERIC_WRITE, // Права на чтение и запись
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	_getch;
	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("CreateFile failed.\n");
		_getch;
		return 1;
	}
	_getch;
	printf("Waiting for parent process to connect...\n");
	ConnectNamedPipe(hPipe, NULL);
	printf("Parent process connected.\n");

	char buffer[3];
	DWORD bytesRead;

	while (WORK && ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) 
	{
		if (buffer[0] == 'M') {
			for (int i = 0; i < MAX_THREADS; i++) {
				if (hThreads[i] == NULL) {
					hThreads[i] = CreateThread(NULL, 0, ThreadMath, (LPVOID)(SIZE_T)i, 0, &IDThread[i]);
					ALLERT = true;
					cout << "THE NEW THREAD HAS BEEN ASSIGNED A NUMBER-" << i << endl;
					ALLERT = false;
					break;
				}

			}
		}
		else if (buffer[0] == 'O') {
			for (int i = 0; i < MAX_THREADS; i++) {
				if (hThreads[i] == NULL) {
					hThreads[i] = CreateThread(NULL, 0, ThreadOutput, (LPVOID)(SIZE_T)i, 0, &IDThread[i]);
					ALLERT = true;
					cout << "THE NEW THREAD HAS BEEN ASSIGNED A NUMBER-" << i << endl;
					ALLERT = false;
					break;
				}
			}
		}
		else if (isdigit(buffer[0])) {
			int index = buffer[0] - '0';

				if (index>= 0 && index< MAX_THREADS && hThreads[index] != NULL) {
					TerminateThread(hThreads[index], 0);
					CloseHandle(hThreads[index]);
					hThreads[index] = NULL;
					ALLERT = true;
					cout << "Thread " << index << " terminated.\n";
					ALLERT = false;
				}
				else {
					ALLERT = true;
					cout << "Invalid number for terminate thread\n";
				}
			}		
		}

	for (int i = 0; i < MAX_THREADS; i++) {
		if (hThreads[i] != NULL) {
			WaitForSingleObject(hThreads[i], INFINITE);
			CloseHandle(hThreads[i]);
		}
	}
	CloseHandle(hPipe);
	cout << "edwbvnjonvje";
	_getch;
	return 0;
}
