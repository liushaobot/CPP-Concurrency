#ifndef THREAD_SAFE_QUEUE_
#define THREAD_SAFE_QUEUE_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
public:

    std::shared_ptr<T> pop() {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            cond_.wait(mlock);
        }
        std::shared_ptr<T> const val(std::make_shared<T> (queue_.front()));
        queue_.pop();
        return val;
    }

    void pop(T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            cond_.wait(mlock);
        }
        item = queue_.front();
        queue_.pop();
    }

    void push(const T& item) {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }

    const T& front() {
        return queue_.front();
    }

    const T& back() {
        return queue_.back();
    }

    bool empty() const {
        std::unique_lock<std::mutex> mlock(mutex_);
        return queue_.empty();
    }

    ThreadSafeQueue()=default;
    ThreadSafeQueue(const ThreadSafeQueue& other) {
        std::unique_lock<std::mutex> mlock(other.mutex_);
        queue_ = other.queue_;
    }
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete; // 禁用赋值操作

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#endif