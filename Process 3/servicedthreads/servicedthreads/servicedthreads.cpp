#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

#define MAX_THREADS 10
LPCWSTR SERVICEDTHREADS = L"C:\\Users\\Tertan41k\\Desktop\\Process 3\\servicethread\\servicethread\\ServiceLog.txt";
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

int main(int argc, char* argv[]) {

    _getch;
    HANDLE hThreads[MAX_THREADS] = { NULL };
    DWORD IDThread[MAX_THREADS] = { NULL };
    CRITICAL_SECTION cs;

    InitializeCriticalSection(&cs);
    HANDLE hPipe = CreateNamedPipe(
        SERVICEDTHREADS,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        512, 512, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("CreateNamedPipe failed.\n");
        _getch();
        return 1;
    }

    printf("Waiting for parent process to connect...\n");
    ConnectNamedPipe(hPipe, NULL);
    printf("Parent process connected.\n");

    char buffer[2];
    DWORD bytesRead;

    while (WORK && ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
        EnterCriticalSection(&cs);

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
            if (index >= 0 && index < MAX_THREADS && hThreads[index] != NULL) {
                TerminateThread(hThreads[index], 0);
                CloseHandle(hThreads[index]);
                hThreads[index] = NULL;
                ALLERT = true;
                cout << "Thread " << index << " terminated.\n";
                ALLERT = false;

                char errorAlert[64] = { "Successful" };
                DWORD bytesWritten;
                if (!WriteFile(hPipe, &errorAlert, sizeof(errorAlert), &bytesWritten, NULL)) {
                    cout << "WriteFile failed.\n";
                    break;
                }
            }
            else {
                cout << "Invalid number for terminate thread\n";
            }
        }
        else {
            if (buffer[0] == 'Q') {
                WORK = false;
            }
        }
        LeaveCriticalSection(&cs);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        if (hThreads[i] != NULL) {
            WaitForSingleObject(hThreads[i], INFINITE);
            CloseHandle(hThreads[i]);
        }
    }
    CloseHandle(hPipe);
    DeleteCriticalSection(&cs);
    return 0;
}
