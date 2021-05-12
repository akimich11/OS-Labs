#include <iostream>
#include <unistd.h>
#include <wait.h>

const int n = 5; //array size
int fd[5][2];

void close_fds(int p_num) {
    for (int i = 0; i < 5; ++i) {
        if (i == p_num)
            close(fd[i][1]);
        else if (i == p_num + 1)
            close(fd[i][0]);
        else {
            close(fd[i][0]);
            close(fd[i][1]);
        }
    }
}

int A() {
    close_fds(0);
    int x[n];
    if (read(fd[0][0], &x, n * sizeof (int)) < 0)
        return 2;
    for (int & i : x)
        i += 12;
    if (write(fd[1][1], &x, n * sizeof (int)) < 0)
        return 3;
    close(fd[0][0]);
    close(fd[1][1]);
    return 0;
}

int M() {
    close_fds(1);
    int x[n];
    if (read(fd[1][0], &x, n * sizeof (int)) < 0)
        return 4;
    for (int & i : x)
        i *= 7;
    if (write(fd[2][1], &x, n * sizeof (int)) < 0)
        return 5;
    close(fd[1][0]);
    close(fd[2][1]);
    return 0;
}

int P() {
    close_fds(2);
    int x[n];
    if (read(fd[2][0], &x, n * sizeof (int)) < 0)
        return 6;
    for (int & i : x)
        i *= i * i;
    if (write(fd[3][1], &x, n * sizeof (int)) < 0)
        return 7;
    close(fd[1][0]);
    close(fd[2][1]);
    return 0;
}
int S() {
    close_fds(3);
    int x[n], sum = 0;
    if (read(fd[3][0], &x, n * sizeof (int)) < 0)
        return 8;
    for (int & i : x)
        sum += i;
    if (write(fd[4][1], &sum, sizeof (int)) < 0)
        return 9;
    close(fd[1][0]);
    close(fd[2][1]);
    return 0;
}

int main() {
    for (auto & i : fd)
        if (pipe(i) == -1)
            return 1;

    int pid1 = fork();
    if (pid1 == 0)
        return A();
    int pid2 = fork();
    if (pid2 == 0)
        return M();
    int pid3 = fork();
    if (pid3 == 0)
        return P();
    int pid4 = fork();
    if (pid4 == 0)
        return S();

    close(fd[0][0]);
    for (int i = 1; i < 4; ++i) {
        close(fd[i][0]);
        close(fd[i][1]);
    }
    close(fd[4][1]);

    int x[n];
    std::cout << "Enter " << n << " int numbers:" << std::endl;
    for (int & i : x)
        std::cin >> i;

    if (write(fd[0][1], &x, n * sizeof (int)) < 0)
        return 10;
    int sum;
    if (read(fd[4][0], &sum, sizeof (int)) < 0)
        return 11;

    std::cout << sum;
    close(fd[0][1]);
    close(fd[4][0]);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    waitpid(pid3, nullptr, 0);
    waitpid(pid4, nullptr, 0);

    return 0;
}
