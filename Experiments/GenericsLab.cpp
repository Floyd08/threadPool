#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>

int pause = 10;
int count = 0;

void hello(std::promise<std::string>&& promise, int count) {
    
    std::string greetings = "Hello";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    std::string message = greetings + " world " + std::to_string(count) + "\n";
    promise.set_value(message);
    //return message; 
}

void counter(std::promise<int>&& promise, int count) {

    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    promise.set_value(count);
}

template <typename T> 
std::future<T> worker(int count) {

    // if (count % 2 == 1) {
    //     std::promise<std::string> promise;
    //     thread(hello, std::move(promise), count);
    //     return promise.get_future();
    // }
    // else {
    //     std::promise<int> promise;
    //     thread(counter, std::move(promise), count);
    //     return promise.get_future();
    // }

    std::promise<T> promise;
    promise.set_value(1);
    return promise.get_future();
}


// std::string workerThread(void (*func)(std::promise<std::string>&&, std::string, int), int count) {

//     std::string greetings = "Hello";

//     std::promise<T> promise;
//     std::future<T> future = promise.get_future();
//     func(std::move(promise), greetings, i);
//     std::string message = std::string(1, label) + ": " + future.get();
//     return message;
// }

int main (void) {

    using std::thread;

    unsigned int maxThreads = thread::hardware_concurrency();
    std::cout << "maxThreads is around: " << maxThreads << "\n";
    thread threads[maxThreads];
    std::future<typename T> results[maxThreads];

    for (int i = 0; i < maxThreads; ++i) {

        results[i] = worker<void>(count);
    }

    for (int i = 0; i < maxThreads; ++i) {
       
        threads[i].join();
    }

    std::cout << "All threads finished\n";
};