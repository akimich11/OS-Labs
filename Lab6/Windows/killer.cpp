#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <Tlhelp32.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

string PROC_TO_KILL;

bool kill(PROCESSENTRY32 pe) {
    HANDLE p = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pe.th32ProcessID);
    return TerminateProcess(p, 9) && CloseHandle(p);
}

bool killPID(int PID) {
    HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    bool hRes = Process32First(ss, &pe);
    while (hRes) {
        if (pe.th32ProcessID == PID)
            return kill(pe) && CloseHandle(ss);
        hRes = Process32Next(ss, &pe);
    }
    CloseHandle(ss);
    return false;
}

bool killName(const string& process_name) {
    HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    bool hRes = Process32First(ss, &pe);
    unsigned int cSize = process_name.size() + 1;
    auto* wc1 = new wchar_t[cSize];
    mbstowcs(wc1, process_name.c_str(), cSize);
    while (hRes) {
        if (!wcscmp(wc1, pe.szExeFile))
            return kill(pe) && CloseHandle(ss);
        hRes = Process32Next(ss, &pe);
    }
    CloseHandle(ss);
    return false;
}

void killEnv() {
    string delimiter = ",", process;
    size_t pos;
    PROC_TO_KILL.erase(remove(PROC_TO_KILL.begin(), PROC_TO_KILL.end(), '\"'), PROC_TO_KILL.end());
    PROC_TO_KILL += ',';
    while ((pos = PROC_TO_KILL.find(delimiter)) != string::npos) {
        process = PROC_TO_KILL.substr(0, pos);
        if (killName(process))
            cout << "killer: " << process << " killed successfully\n";
        else
            cout << "killer: error killing " << process << endl;
        PROC_TO_KILL.erase(0, pos + delimiter.length());
    }
}

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        cout << "Usage:\n" << argv[0] << " --id <PID>" << '\n';
        cout << argv[0] << " --name <process name>" << '\n';
        system("pause");
        return 1;
    }
    string ps_to_kill;
    if (getenv("PROC_TO_KILL"))
        PROC_TO_KILL = getenv("PROC_TO_KILL");
    cout << "killer: killing apps in PROC_TO_KILL...\n";
    killEnv();
    cout << "killer: killing other apps...\n";
    stringstream ss(argv[2]);
    if (!strcmp(argv[1], "--id")) {
        int pid;
        if (ss >> pid && killPID(pid))
            cout << "killer: process with pid " << pid << " killed successfully\n";
        else
            cout << "killer: something went wrong\n";
        system("pause");
        return 0;
    }

    if (!strcmp(argv[1], "--name")) {
        string name;
        ss >> name;
        if (killName(name))
            cout << "killer: " << name << " killed successfully\n";
        else
            cout << "killer: error killing " << name << endl;
        system("pause");
        return 0;
    }
}