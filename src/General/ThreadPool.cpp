#include<ThreadPool.h>

// From https://www.geeksforgeeks.org/thread-pool-in-cpp/

RedisReImp::General::ThreadPool::ThreadPool(size_t numThreads) {
    // creating worker threads
    stop_ = false;
    for (size_t i = 0; i < numThreads; ++i) { 
            threads_.emplace_back([this] { 
                while (true) { 
                    std::function<void()> task; 
                    // The reason for putting the below code 
                    // here is to unlock the queue before 
                    // executing the task so that other 
                    // threads can perform enqueue tasks 
                    { 
                        // Locking the queue so that data 
                        // can be shared safely 
                        
                        std::unique_lock<std::mutex> lock( 
                            queueMutex_);
                        

                        // Waiting until there is a task to 
                        // execute or the pool is stopped 
                        cv_.wait(lock, [this] {
                            return !tasks_.empty() || stop_; 
                        }); 
                        

                        // exit the thread in case the pool 
                        // is stopped and there are no tasks 
                        if (stop_ && tasks_.empty()) { 
                            return; 
                        } 
  
                        // Get the next task from the queue 
                        task = move(tasks_.front()); 
                        tasks_.pop(); 
                    } 
                    
                    task();
                } 
            }); 
        } 

}


RedisReImp::General::ThreadPool::~ThreadPool() {
    { 
        // Lock the queue to update the stop flag safely 
        std::unique_lock<std::mutex> lock(queueMutex_); 
        stop_ = true; 
    } 
  
    // Notify all threads 
    cv_.notify_all(); 

    // Joining all worker threads to ensure they have 
    // completed their tasks 
    for (auto& thread : threads_) { 
        thread.join(); 
    }

}


void RedisReImp::General::ThreadPool::enqueue(std::function<void()> task) {
    { 
        General::RuntimeCounter::moduleStart("Lock");
        std::unique_lock<std::mutex> lock(queueMutex_); 
        
        tasks_.emplace(std::move(task)); 
        
    }

    cv_.notify_one(); 
    General::RuntimeCounter::moduleStart("LockEnd");
}

void RedisReImp::General::ThreadPool::enqueue(std::vector<std::function<void()>> & tasks) {
    { 
        std::unique_lock<std::mutex> lock(queueMutex_); 
        for (auto task:tasks) {
            tasks_.emplace(std::move(task));
        }
    }
    cv_.notify_one(); 
    
}