#ifndef THREADSAFEDEQUE_H
#define THREADSAFEDEQUE_H

#include <deque>
#include <mutex>
#include <optional>
#include <utility>

template <typename T>
class ThreadSafeDeque
{
public:
    void push_back(const T& item)
    {
        std::scoped_lock lock(mutex_);
        deque_.push_back(item);
    }

    std::optional<T> pop_back()
    {
        std::scoped_lock lock(mutex_);
        if (deque_.empty())
            return {};
        auto ret = std::move(deque_.back());
        deque_.pop_back();
        return {ret};
    }

    void push_front(const T& item)
    {
        std::scoped_lock lock(mutex_);
        deque_.push_front(item);
    }

    std::optional<T> pop_front()
    {
        std::scoped_lock lock(mutex_);
        if (deque_.empty())
            return {};
        auto ret = std::move(deque_.front());
        deque_.pop_front();
        return {ret};
    }

    bool empty() const
    {
        std::scoped_lock lock(mutex_);
        return deque_.empty();
    }

private:
    std::deque<T> deque_;
    mutable std::mutex mutex_;
};

#endif// THREADSAFEDEQUE_H
