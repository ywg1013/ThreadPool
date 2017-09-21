//
// Created by 杨文广 on 2017/9/15.
//

#include "ThreadPool.h"


ThreadPool::ThreadPool(int threadCount) :
        m_stop(false) {
    for (size_t i = 0; i < threadCount; ++i) {

        m_workers.emplace_back(
                [this] {
                    for (;;) {

                        std::unique_lock<std::mutex> lock(m_queue_mutex);
                        m_condition.wait(lock, [this] { return m_stop || !m_tasks.empty(); });
                        if (m_stop && m_tasks.empty())
                            return;
                        std::function<void()> task = std::move(m_tasks.front());
                        m_tasks.pop();
                        lock.unlock();

                        task();
                    }
                }
        );

    }
}


ThreadPool::~ThreadPool() {

    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_stop = true;
    }
    m_condition.notify_all();
    for (std::thread &worker: m_workers) {
        worker.join();
    }

}
