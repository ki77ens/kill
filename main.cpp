#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool terminateProc(const std::string& procName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        cout << "Couldn't create process snapshot!" << endl;
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    bool terminated = false;
    if (Process32First(hSnap, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, procName.c_str()) == 0) {
                HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProc) {
                    if (TerminateProcess(hProc, 0)) {
                        cout << "Process " << procName << " terminated." << endl;
                        terminated = true;
                    }
                    else {
                        cout << "Could not terminate process: " << procName << endl;
                    }
                    CloseHandle(hProc);
                }
                break;
            }
        } while (Process32Next(hSnap, &pe32));
    }

    CloseHandle(hSnap);

    if (!terminated) {
        cout << "No processes matching name: " << procName << endl;
    }

    return terminated;
}

int main() {
    vector<string> procsToTerm = { "notepad.exe", "chrome.exe", "firefox.exe" };

    bool anyTermed= false;
    for (const auto& procName : procsToTerm) {
        if (terminateProc(procName)) {
            anyTerminated = true;
        }
    }

    if (!anyTermed) {
        cout << "No processes were terminated." << endl;
    }

    return 0;
}
