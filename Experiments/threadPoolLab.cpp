#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>
#include <functional>

template <typename T>
class ThreadPool {
    public:
        std::future<T> pushTask(std::packaged_task<T()> task) {
            std::future<T> result = task.get_future();
            taskQueue.push(std::move(task));
            return result;
        }
        void worker() {
            std::packaged_task<T()> task;
            task = std::move(taskQueue.front());
            taskQueue.pop();
            std::cout << "worker pulled a task from the queue\n";
            task();
        }
        ThreadPool() {};

    private:
        std::vector<std::thread> threads;
        std::queue<std::packaged_task<T()>> taskQueue;
};

int pause = 3000;

std::string hello(std::string greetings, int count) {
    
    std::string message = "";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    message = greetings + " world " + std::to_string(count) + "\n";
    return message; 
}

int main(void) {

    unsigned int maxThreads = 4;
    std::string greetings = "Hello";

    ThreadPool<std::string> tPool;
    std::future<std::string> results[maxThreads];

    for(int i = 0; i < maxThreads; ++i) {
        // auto taskPack = std::bind(hello, greetings, i);
        // std::packaged_task<std::string()> task(taskPack);
        std::packaged_task<std::string()> task(std::bind(&hello, greetings, i));
        //std::future<std::string> result = tPool.pushTask(std::move(task));
        results[i] = tPool.pushTask(std::move(task));
    }
    std::cout << "All tasks pushed to queue\n";

    for(int i = 0; i < maxThreads; ++i) {
        tPool.worker();
        std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    }
    std::cout << "All Threads started\n";

    for(int i = 0; i < maxThreads; ++i) {
        std::string message = results[i].get();
        std::cout << message;
    }
    std::cout << "All threads finished\n";
    
    return 0;
}