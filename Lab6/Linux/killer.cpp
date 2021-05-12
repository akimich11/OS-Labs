#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <csignal>
#include <dirent.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

string PROC_TO_KILL;

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

bool killPID(int PID) {
	return !kill(PID, SIGTERM);
}

bool killName(const string& process_name) {
	int pid = getPIDByName(process_name);
	return pid != -1 && killPID(pid);
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

int main(int argc, char *argv[]) {
	if (argc <= 2) {
		cout << "Usage:\n" << argv[0] << " --id <PID>" << '\n';
		cout << argv[0] << " --name <process name>" << '\n';
		return 1;
	}
	string ps_to_kill;
    if (getenv("PROC_TO_KILL"))
	    PROC_TO_KILL = getenv("PROC_TO_KILL");
    killEnv();

	stringstream ss(argv[2]);
	if (!strcmp(argv[1], "--id")) {
		int pid;
		if(ss >> pid && killPID(pid))
			cout << "killer: process with pid " << pid << " killed successfully\n";
		else
			cout << "killer: something went wrong\n";
		return 0;
	}
	
	if (!strcmp(argv[1], "--name")) {
		string name;
		ss >> name;
		if(killName(name))
            cout << "killer: " << name << " killed successfully\n";
		else
            cout << "killer: error killing " << name << endl;
		return 0;
	}
}