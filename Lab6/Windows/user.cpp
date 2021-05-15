#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <Tlhelp32.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>

using namespace std;

int getPIDByName(const string& process_name) {
    HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    bool hRes = Process32First(ss, &pe);
    unsigned int cSize = process_name.size() + 1;
    auto* wc1 = new wchar_t[cSize];
    mbstowcs(wc1, process_name.c_str(), cSize);
    while (hRes) {
        if (!wcscmp(wc1, pe.szExeFile))
            return pe.th32ProcessID;
        hRes = Process32Next(ss, &pe);
    }
    CloseHandle(ss);
    return -1;
}

int main() {
    const char* apps = "firefox.exe,Telegram.exe";
    SetEnvironmentVariableA("PROC_TO_KILL", apps);

    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { nullptr };

    CreateProcess(L"C:\\WINDOWS\\system32\\notepad.exe", nullptr, nullptr, nullptr, false, 0,
                  nullptr, nullptr, &si, &pi);
    CreateProcess(L"C:\\WINDOWS\\system32\\mspaint.exe", nullptr, nullptr, nullptr, false, 0,
                  nullptr, nullptr, &si, &pi);


    if (getPIDByName("mspaint.exe") != -1) {
        stringstream ss;
        ss << pi.dwProcessId;
        string text = R"(D:\Workspace\CLion\Algorithms\killer.exe --id )" + ss.str();
        auto* tmp = new wchar_t[text.size() + 1];
        mbstowcs(tmp, text.c_str(), text.size());
        CreateProcess(nullptr, tmp, nullptr, nullptr, false, 0,
                      nullptr, nullptr, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        cout << "-------------------------------------------------------------------" << endl;
        if (getPIDByName("notepad.exe") == -1)
            cout << "user: killing by id is checked, success" << endl;
        else
            cout << "user: killing by id is checked, error" << endl;
    }
    else
        cout << "user: killing by id is not checked" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    if (getPIDByName("notepad.exe") != -1) {
        const char* text = R"(D:\Workspace\CLion\Algorithms\killer.exe --name notepad.exe)";
        auto* tmp = new wchar_t[61];
        mbstowcs(tmp, text, 60);

        CreateProcess(NULL, tmp, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        cout << "-------------------------------------------------------------------" << endl;
        if (getPIDByName("mspaint.exe") == -1)
            cout << "user: killing by name is checked, success" << endl;
        else
            cout << "user: killing by name is checked, error" << endl;
    }
    else
        cout << "user: killing by name is not checked" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    SetEnvironmentVariableA("PROC_TO_KILL", nullptr);
    system("pause");
    return 0;
}