#ifndef THREADSAFEDEQUE_H
#define THREADSAFEDEQUE_H

#include <deque>
#include <mutex>
#include <optional>
#include <utility>

template <typename T>
class ThreadSafeDeuqeue
{
public:
    std::optional<T> pop()
    {
        std::scoped_lock lock(mutex_);
        if (deque_.empty())
            return {};
        auto ret = std::move(deque_.back());
        return {ret};
    }

private:
    std::deque<T> deque_;
    std::mutex mutex_;
};

#endif// THREADSAFEDEQUE_H
