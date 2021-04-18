#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <chrono>
#include <queue>
#include <unordered_map>

using namespace std;

const int MAXN = 1000;

int** matrix1, ** matrix2;
queue<unordered_map<string, int>> q1, q2, q3;
CRITICAL_SECTION cs_ans, cs_block[MAXN][MAXN], cs_q1, cs_q2, cs_q3;

void printAnswer(int** a, int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << setw(5) << a[i][j];
        cout << '\n';
    }
}
void clearMatrix(int** a, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = 0;
}

void initQ1(int n, int m) {
    unordered_map<string, int> tmp;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            tmp.clear();
            tmp.insert(make_pair("a", i));
            tmp.insert(make_pair("b", j));
            tmp.insert(make_pair("m", m));
            q1.push(tmp);
        }
}
void initQ2(int n, int m) {
    unordered_map<string, int> tmp;
    for (int i = 0; i < m; i++)
    {
        tmp.clear();
        tmp.insert(make_pair("a", i));
        tmp.insert(make_pair("n", n));
        q2.push(tmp);
    }
}
void initQ3(int n, int m, int k) {
    unordered_map<string, int> tmp;
    int* s1 = new int[k], * s2 = new int[k];
    int n1 = n, m1 = m, n2 = 0;
    for (int i = 0; i < k - 1; i++)
    {
        s1[i] = n / k;
        n1 -= n / k;
        s2[i] = m / k;
        m1 -= m / k;
    }
    s1[k - 1] = n1;
    s2[k - 1] = m1;
    n1 = 0;

    for (int i = 0; i < k; i++)
    {
        m1 = 0;
        for (int j = 0; j < k; j++)
        {
            n2 = 0;
            for (int l = 0; l < k; l++)
            {
                tmp.clear();
                tmp.insert(make_pair("a1", n1));
                tmp.insert(make_pair("a2", m1));
                tmp.insert(make_pair("b1", m1));
                tmp.insert(make_pair("b2", n2));
                tmp.insert(make_pair("n1", s1[i]));
                tmp.insert(make_pair("m1", s2[j]));
                tmp.insert(make_pair("n2", s2[j]));
                tmp.insert(make_pair("m2", s1[l]));
                tmp.insert(make_pair("k", k));
                q3.push(tmp);

                n2 += s1[l];
            }
            m1 += s2[j];
        }
        n1 += s1[i];
    }
}

void dummyMultiply(int** arr, int n, int m) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < m; k++)
                arr[i][j] += matrix1[i][k] * matrix2[k][j];
}
DWORD WINAPI smartMultiply1(const LPVOID args) {
    int** answer = (int**)args;
    unordered_map<string, int> u_map_matrix;
    int a, b, n, m;
    int* row, * column;
    while (true)
    {
        EnterCriticalSection(&cs_q1);
        if (q1.empty()) {
            LeaveCriticalSection(&cs_q1);
            break;
        }
        u_map_matrix = q1.front(); q1.pop();
        LeaveCriticalSection(&cs_q1);

        m = u_map_matrix.find("m")->second;
        row = new int[m];
        column = new int[m];
        a = u_map_matrix.find("a")->second;
        b = u_map_matrix.find("b")->second;
        for (int i = 0; i < m; i++)
        {
            row[i] = matrix1[a][i];
            column[i] = matrix2[i][b];
        }
        answer[a][b] = 0;
        for (int i = 0; i < m; i++)
            answer[a][b] += row[i] * column[i];
    }
    ExitThread(0);
}
DWORD WINAPI smartMultiply2(const LPVOID args) {
    int** answer = (int**)args;
    unordered_map<string, int> u_map_matrix;
    int a, n;
    int* row, * col;
    while (true)
    {
        EnterCriticalSection(&cs_q2);
        if (q2.empty()) {
            LeaveCriticalSection(&cs_q2);
            break;
        }
        u_map_matrix = q2.front(); q2.pop();
        LeaveCriticalSection(&cs_q2);

        a = u_map_matrix.find("a")->second;
        n = u_map_matrix.find("n")->second;
        row = new int[n];
        col = new int[n];
        for (int i = 0; i < n; i++)
        {
            row[i] = matrix1[i][a];
            col[i] = matrix2[a][i];
        }

        int** tmp = new int* [n];
        for (int i = 0; i < n; i++)
        {
            tmp[i] = new int[n];
            for (int j = 0; j < n; j++)
                tmp[i][j] = col[i] * row[j];
        }
        EnterCriticalSection(&cs_ans);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                answer[i][j] += tmp[i][j];
        LeaveCriticalSection(&cs_ans);
    }
    ExitThread(0);
}
DWORD WINAPI smartMultiply3(const LPVOID args) {
    int** answer = (int**)args;
    unordered_map<string, int> u_map_matr;
    int a1, b1, n1, m1, a2, b2, n2, m2, k;
    int** array1, ** array2;
    while (true)
    {
        EnterCriticalSection(&cs_q3);
        if (q3.empty()) {
            LeaveCriticalSection(&cs_q3);
            break;
        }
        u_map_matr = q3.front(); q3.pop();
        LeaveCriticalSection(&cs_q3);

        n1 = u_map_matr.find("n1")->second;
        n2 = u_map_matr.find("n2")->second;
        a1 = u_map_matr.find("a1")->second;
        b1 = u_map_matr.find("b1")->second;
        array1 = new int* [n1];
        array2 = new int* [n2];
        m1 = u_map_matr.find("m1")->second;
        for (int i = 0; i < n1; i++)
        {
            array1[i] = new int[m1];
            for (int j = 0; j < m1; j++)
                array1[i][j] = matrix1[a1 + i][b1 + j];
        }
        a2 = u_map_matr.find("a2")->second;
        b2 = u_map_matr.find("b2")->second;
        m2 = u_map_matr.find("m2")->second;
        for (int i = 0; i < n2; i++)
        {
            array2[i] = new int[m2];
            for (int j = 0; j < m2; j++)
                array2[i][j] = matrix2[a2 + i][b2 + j];
        }
        int** tmp = new int* [n1];
        for (int i = 0; i < n1; i++)
        {
            tmp[i] = new int[m2];
            for (int j = 0; j < m2; j++)
            {
                tmp[i][j] = 0;
                for (int r = 0; r < n2; r++)
                    tmp[i][j] += array1[i][r] * array2[r][j];
            }
        }
        k = u_map_matr.find("k")->second;
        EnterCriticalSection(&(cs_block[a1 / k][b2 / k]));
        for (int i = 0; i < n1; i++)
        {
            for (int j = 0; j < m2; j++)
                answer[a1 + i][b2 + j] += tmp[i][j];
        }
        LeaveCriticalSection(&(cs_block[a1 / k][b2 / k]));
    }
    ExitThread(0);
}

int main() {
    unordered_map<string, int> tmp;
    ifstream in("input.txt");
    int n, m, k;
    
    // first matrix is N x M, second is N x M
    in >> n >> m >> k;
    matrix1 = new int*[n];
    matrix2 = new int*[m];
    int** answer = new int*[n];

    for (int i = 0; i < n; i++)
    {
        matrix1[i] = new int[m];
        for (int j = 0; j < m; j++)
            in >> matrix1[i][j];
    }
    for (int i = 0; i < m; i++)
    {
        matrix2[i] = new int[n];
        for (int j = 0; j < n; j++)
            in >> matrix2[i][j];
    }
    in.close();
    for (int i = 0; i < n; i++)
    {
        answer[i] = new int[n];
        for (int j = 0; j < n; j++)
            answer[i][j] = 0;
    }

    InitializeCriticalSection(&cs_ans);
    InitializeCriticalSection(&cs_q1);
    InitializeCriticalSection(&cs_q2);
    InitializeCriticalSection(&cs_q3);
    initQ1(n, m);
    initQ2(n, m);
    initQ3(n, m, k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++)
            InitializeCriticalSection(&cs_block[i][j]);

    auto start = chrono::steady_clock::now();
    dummyMultiply(answer, n, m);
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Dummy Multiply: " << elapsed.count() << " ms" << "\n\n";
    printAnswer(answer, n);
    clearMatrix(answer, n);

    auto* mutexes = new HANDLE[k];
    start = chrono::steady_clock::now();
    for (int i = 0; i < k; i++)
        mutexes[i] = CreateThread(nullptr, 0, smartMultiply1, static_cast<void*>(answer), 0, nullptr);
    WaitForMultipleObjects(k, mutexes, true, INFINITE);
    end = chrono::steady_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nMulti-threaded multiply (row by col): " << elapsed.count() << " ms" << "\n\n";
    printAnswer(answer, n);
    clearMatrix(answer, n);

    start = chrono::steady_clock::now();
    for (int i = 0; i < k; i++)
        mutexes[i] = CreateThread(nullptr, 0, smartMultiply2, static_cast<void*>(answer), 0, nullptr);
    WaitForMultipleObjects(k, mutexes, true, INFINITE);
    end = chrono::steady_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nMulti-threaded multiply (col by row): " << elapsed.count() << " ms" << "\n\n";
    printAnswer(answer, n);
    clearMatrix(answer, n);

    start = chrono::steady_clock::now();
    for (int i = 0; i < k; i++)
        mutexes[i] = CreateThread(nullptr, 0, smartMultiply3, static_cast<void*>(answer), 0, nullptr);
    WaitForMultipleObjects(k, mutexes, true, INFINITE);
    end = chrono::steady_clock::now();
    elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "\nMulti-threaded multiply (block by block): " << elapsed.count() << " ms" << "\n\n";
    printAnswer(answer, n);

    return 0;
}