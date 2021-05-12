#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <dirent.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int getPIDByName(const string& process_name) {
    int pid = -1;
    DIR *dp = opendir("/proc");
    if (dp) {
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp))) {
            int id = atoi(dirp->d_name);
            if (id > 0) {
                string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty()) {
                    size_t pos = cmdLine.find('\0');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    pos = cmdLine.rfind('/');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    if (process_name == cmdLine)
                        pid = id;
                }
            }
        }
    }
    closedir(dp);
    return pid;
}

int main() {
    setenv("PROC_TO_KILL", "firefox,Telegram", 1);
    system("g++ /home/akim/repos/OS-Labs/Lab6/Linux/killer.cpp -o killer");
    string app1 = "gedit", app2 = "gnome-calculator";

    string launch_command = app1 + "&" + app2 + "&";
    system(launch_command.c_str());

    int pid1 = getPIDByName(app1);
    int pid2 = getPIDByName(app2);
    if (pid1 == -1 || pid2 == -1)
        return 1;

    usleep(2000000);

    string test_command1 = "./killer --id " + to_string(pid1);
    system(test_command1.c_str());
    if (getPIDByName(app1) == -1 && getPIDByName("Telegram") == -1 && getPIDByName("firefox") == -1)
        cout << "user: kill test by id passed" << endl;
    else
        cout << "user: error killing by id" << endl;

    string test_command2 = "./killer --name " + app2;
    system(test_command2.c_str());
    if (getPIDByName(app2) == -1)
        cout << "user: kill test by name passed" << endl;
    else
        cout << "user: error killing by name" << endl;

    unsetenv("PROC_TO_KILL");
    return 0;
}
