#include <iostream>
#include "ThreadPool.h"

#if 1

int main() {
    std::cout << "Hello, World!" << std::endl;
    ThreadPool threadPool;
    auto test = threadPool.enqueue([](int a, int b) {
        std::cout << "id = " << std::this_thread::get_id() << " start test\n";
        std::this_thread::sleep_for(std::chrono::seconds(13));
        return a + b;
    }, 8, 9);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto test1 = threadPool.enqueue([](int a, int b) {
        std::cout << "id = " << std::this_thread::get_id() << " start test1 \n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return a + b;
    }, 18, 9);


    std::cout << test1.get() << std::endl;
    std::cout << "Hello, World!" << std::endl;
    std::cout << test.get() << std::endl;

    return 0;
}

#else

#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>
#include <chrono>

// unique function to avoid disambiguating the std::pow overload set
int f(int x, int y) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return std::pow(x, y);
}

void task_lambda() {
    std::packaged_task<int(int, int)> task([](int a, int b) {
        return std::pow(a, b);
    });
    std::future<int> result = task.get_future();

    task(2, 9);

    std::cout << "task_lambda:\t" << result.get() << '\n';
}

void task_bind() {
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();

    task();

    std::cout << "task_bind:\t" << result.get() << '\n';
}

void task_thread() {
    std::packaged_task<int(int, int)> task(f);
    std::future<int> result = task.get_future();
    std::cout << "yangwg111\n";
    std::thread task_td(std::move(task), 2, 10);
    std::cout << "yangwg\n";

    int ret = result.get();
    std::cout << "task_thread:\t" << ret << '\n';
    task_td.join();
    std::cout << "fdsfd\n";
}

int main() {
    task_lambda();
    task_bind();
    task_thread();
}
#endif