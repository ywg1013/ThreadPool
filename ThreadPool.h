//
// Created by 杨文广 on 2017/9/15.
//

#ifndef HELLO_THREADPOOL_H
#define HELLO_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <future>


class ThreadPool {
public:

    ThreadPool(int threadCount = std::thread::hardware_concurrency());

    virtual ~ThreadPool();

    template<typename F, typename... Args>
    auto enqueue(F &&f, Args &&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> m_workers;
    // the task queue
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
        m_tasks.emplace([task]() { (*task)(); });
    }

    m_condition.notify_one();
    return res;
}


#endif //HELLO_THREADPOOL_H
