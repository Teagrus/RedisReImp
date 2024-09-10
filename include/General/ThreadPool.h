#pragma once

#include <RedisReImpGeneral.h>
#include <condition_variable> 
#include <functional> 
#include <iostream> 
#include <mutex> 
#include <queue> 
#include <thread> 


namespace RedisReImp::General {
// namespace ---------


class ThreadPool {
public:

    ThreadPool(size_t numThreads = std::thread::hardware_concurrency());

    ~ThreadPool();

    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queueMutex_;
    std::condition_variable cv_;
    bool stop_;
};



// end namespace ---------
}
