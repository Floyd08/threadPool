#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>

int pause = 10;

std::string hello(std::string greetings, int count) {
    
    std::string message = "";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    message = greetings + " world " + std::to_string(count) + "\n";
    //promise.set_value(message);
    return message; 
}

int main(void) {

    using std::thread;

    unsigned int maxThreads = thread::hardware_concurrency();
    std::cout << "maxThreads is around: " << maxThreads << "\n";
    std::string greetings = "Hello";
    std::thread threads[maxThreads];
    std::future<std::string> results[maxThreads];

    for (int i = 0; i< maxThreads; ++i) {

        std::packaged_task<std::string(std::string, int)> task(hello);
        results[i] = task.get_future();
        threads[i] = thread(std::move(task), greetings, i);
    }

    for (int i = 0; i < maxThreads; ++i) {
        
        std::string message = results[i].get();
        std::cout << message;
        threads[i].join();
    }

    std::cout << "All threads finished\n";
}