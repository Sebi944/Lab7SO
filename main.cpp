#include <iostream>
#include <windows.h>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int NUM_PROCESSES = 10;
HANDLE resourceMutex;
queue<pair<HANDLE, string>> waitingQueue;
HANDLE processHandles[NUM_PROCESSES];
HANDLE queueSemaphore;

void accessResource(const char* color) {
    WaitForSingleObject(resourceMutex, INFINITE);
    cout << "Process " << GetCurrentProcessId() << " (" << color << ") is accessing the resource.\n";
    Sleep(1000);
    cout << "Process " << GetCurrentProcessId() << " (" << color << ") has finished using the resource.\n";
    ReleaseMutex(resourceMutex);
    ReleaseSemaphore(queueSemaphore, 1, NULL);
}

DWORD WINAPI processThread(LPVOID param) {
    const char* color = static_cast<const char*>(param);

    WaitForSingleObject(queueSemaphore, INFINITE);
    accessResource(color);

    return 0;
}

int main() {
    srand(time(0));

    resourceMutex = CreateMutex(NULL, FALSE, NULL);
    queueSemaphore = CreateSemaphore(NULL, 1, NUM_PROCESSES, NULL);

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        const char* color = (rand() % 2 == 0) ? "white" : "black";
        processHandles[i] = CreateThread(NULL, 0, processThread, (LPVOID)color, 0, NULL);
    }

    WaitForMultipleObjects(NUM_PROCESSES, processHandles, TRUE, INFINITE);

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        CloseHandle(processHandles[i]);
    }

    CloseHandle(resourceMutex);
    CloseHandle(queueSemaphore);

    return 0;
}
