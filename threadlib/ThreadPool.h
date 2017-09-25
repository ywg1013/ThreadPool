//
// Created by 杨文广 on 2017/9/15.
//

#ifndef HELLO_THREADPOOL_H
#define HELLO_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <future>
#include "Singleton.h"

class ThreadPool {

DECLARE_SINGLETON(ThreadPool)

private:

    explicit ThreadPool(int threadCount = std::thread::hardware_concurrency());

    virtual ~ThreadPool();

public:
    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

private:

    std::vector<std::thread> m_workers;
    std::queue<std::function<void()> > m_tasks;

    // synchronization
    std::mutex m_queue_mutex;
    std::condition_variable m_condition;
    bool m_stop;

};

// add new work item to the pool
template<typename F, typename... Args>
auto ThreadPool::enqueue(F &&f, Args &&... args)
-> std::future<typename std::result_of<F(Args...)>::type> {

    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        if (m_stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        m_tasks.emplace([task]() { (*task)(); });
    }

    m_condition.notify_one();
    return res;
}


#endif //HELLO_THREADPOOL_H
