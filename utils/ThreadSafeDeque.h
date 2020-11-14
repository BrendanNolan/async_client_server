#ifndef THREADSAFEDEQUE_H
#define THREADSAFEDEQUE_H

#include <deque>
#include <optional>

template <typename T>
class ThreadSafeDeuqeue
{
public:
    std::optional<T> pop()
    {

    }

private:
    std::deque<T> deque_;
};

#endif// THREADSAFEDEQUE_H
