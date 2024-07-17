#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool terminateProcessByName(const string& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        cerr << "failed to create process snapshot." << endl;
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    bool processTerminated = false;
    if (Process32First(snapshot, &processEntry)) {
        do {
            if (_stricmp(processEntry.szExeFile, processName.c_str()) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
                if (hProcess && TerminateProcess(hProcess, 0)) {
                    cout << "terminated process: " << processName << endl;
                    processTerminated = true;
                }
                else {
                    cerr << "failed to terminate process : " << processName << endl;
                }
                CloseHandle(hProcess);
                break;
            }
        } while (Process32Next(snapshot, &processEntry));
    }
    else {
        cerr << "failed to retrieve first process." << endl;
    }

    CloseHandle(snapshot);

    if (!processTerminated) {
        cerr << "no processes matching name: " << processName << endl;
    }

    return processTerminated;
}

int main() {
    vector<string> processesToTerminate = { "notepad.exe", "chrome.exe", "firefox.exe" }; // add whatever you want

    bool anyProcessTerminated = false;
    for (const auto& processName : processesToTerminate) {
        if (terminateProcessByName(processName)) {
            anyProcessTerminated = true;
        }
    }

    if (!anyProcessTerminated) {
        cerr << "no processes were successfully terminated." << endl;
    }

    return 0;
}
