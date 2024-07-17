#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>

bool terminateProc(const std::string& procName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Couldn't create process snapshot!" << std::endl;
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
                        std::cout << "Process " << procName << " terminated." << std::endl;
                        terminated = true;
                    }
                    else {
                        std::cerr << "Could not terminate process: " << procName << std::endl;
                    }
                    CloseHandle(hProc);
                }
                break;
            }
        } while (Process32Next(hSnap, &pe32));
    }

    CloseHandle(hSnap);

    if (!terminated) {
        std::cerr << "No processes matching name: " << procName << std::endl;
    }

    return terminated;
}

int main() {
    std::vector<std::string> procsToTerminate = { "notepad.exe", "chrome.exe", "firefox.exe" };

    bool anyTerminated = false;
    for (const auto& procName : procsToTerminate) {
        if (terminateProc(procName)) {
            anyTerminated = true;
        }
    }

    if (!anyTerminated) {
        std::cerr << "No processes were terminated." << std::endl;
    }

    return 0;
}
