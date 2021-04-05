#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>
using namespace std;

template<class T>
class BufferedChannel {
private:
    mutable mutex locker;
    queue<T> elements;
    condition_variable notifier;
    int buffer_size;
    bool is_opened = false;
public:
    explicit BufferedChannel(int size) {
        buffer_size = size;
        is_opened = true;
    }

    void Send(T value) {
        unique_lock<mutex> u_lock(locker);
        notifier.wait(u_lock, [&]() {return elements.size() != buffer_size; });
        if (!is_opened)
            throw runtime_error("panic!");
        elements.push(value);
        u_lock.unlock();
        notifier.notify_all();
    }

    std::pair<T, bool> Recv() {
        unique_lock<mutex> u_lock(locker);
        notifier.wait(u_lock, [&]() {return !elements.empty(); });
        T element = elements.front();
        elements.pop();
        u_lock.unlock();
        notifier.notify_all();
        return make_pair(element, is_opened);
    }

    void Close() {
        unique_lock<mutex> u_lock(locker);
        is_opened = false;
        u_lock.unlock();
        notifier.notify_all();
    }
};