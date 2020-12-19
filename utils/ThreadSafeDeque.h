#ifndef THREADSAFEDEQUE_H
#define THREADSAFEDEQUE_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <utility>

namespace utils
{

template<typename T> class ThreadSafeDeque
{
public:
    std::size_t size() const
    {
        std::scoped_lock lock{ mutex_ };
        return deque_.size();
    }

    T* back()
    {
        std::scoped_lock lock{ mutex_ };
        return (!deque_.empty()) ? &(deque_.back()) : nullptr;
    }

    const T* back() const
    {
        std::scoped_lock lock{ mutex_ };
        return (!deque_.empty()) ? &(deque_.back()) : nullptr;
    }

    void push_back(const T& item)
    {
        std::scoped_lock lock{ mutex_ };
        deque_.push_back(item);
        condVar_.notify_one();
    }

    void push_back(T&& item)
    {
        std::scoped_lock lock{ mutex_ };
        deque_.push_back(std::move(item));
        condVar_.notify_one();
    }

    std::optional<T> try_pop_back()
    {
        std::scoped_lock lock{ mutex_ };
        if (deque_.empty())
            return {};
        return { pop_back_private() };
    }

    T wait_and_pop_back()
    {
        std::unique_lock lock{ mutex_ };
        condVar_.wait(lock, [this]() { return !deque_.empty(); });
        return pop_back_private();
    }

    T* front()
    {
        std::scoped_lock lock{ mutex_ };
        return (!deque_.empty()) ? &(deque_.front()) : nullptr;
    }

    const T* front() const
    {
        std::scoped_lock lock{ mutex_ };
        return (!deque_.empty()) ? &(deque_.front()) : nullptr;
    }

    void push_front(const T& item)
    {
        std::scoped_lock lock{ mutex_ };
        deque_.push_front(item);
        condVar_.notify_one();
    }

    void push_front(T&& item)
    {
        std::scoped_lock lock{ mutex_ };
        deque_.push_front(std::move(item));
        condVar_.notify_one();
    }

    std::optional<T> try_pop_front()
    {
        std::scoped_lock lock{ mutex_ };
        if (deque_.empty())
            return {};
        return { pop_front_private() };
    }

    T wait_and_pop_front()
    {
        std::unique_lock lock{ mutex_ };
        condVar_.wait(lock, [this]() { return !deque_.empty(); });
        return pop_front_private();
    }

    bool empty() const
    {
        std::scoped_lock lock{ mutex_ };
        return deque_.empty();
    }

    void clear() 
    { 
        std::scoped_lock lock{ mutex_ };
        deque_.clear();
    }

private:
    T pop_back_private()
    {
        auto ret = std::move(deque_.back());
        deque_.pop_back();
        return ret;
    }

    T pop_front_private()
    {
        auto ret = std::move(deque_.front());
        deque_.pop_front();
        return ret;
    }

private:
    std::deque<T> deque_;

    std::condition_variable condVar_;
    mutable std::mutex mutex_;
};

}// namespace utils

#endif// THREADSAFEDEQUE_H
