#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>

int pause = 10;
int count = 0;

void hello(std::promise<std::string>&& promise, std::string greetings, int count) {
    
    std::string message = "";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    message = greetings + " world " + std::to_string(count) + "\n";
    promise.set_value(message);
    //return message; 
}
template <typename T>
void workerThread(void (*func)(std::promise<T>&&, std::string, int), char label) {

    std::string greetings = "Hello";

    for (int i = 0; i < 4; ++i) {
        std::promise<T> promise;
        std::future<T> future = promise.get_future();
        func(std::move(promise), greetings, i);
        std::string message = std::string(1, label) + ": " + future.get();
        std::cout << message;
    }
}

int main (void) {

    using std::thread;

    unsigned int maxThreads = thread::hardware_concurrency();
    std::cout << "maxThreads is around: " << maxThreads << "\n";
    thread threads[maxThreads];
    char label = 'a';

    for (int i = 0; i < maxThreads; ++i) {
        
        void (*hi)(std::promise<std::string>&&, std::string, int);
        threads[i] = thread(workerThread<std::string>, hello, label);
        ++label;
    }

    for (int i = 0; i < maxThreads; ++i) {
        threads[i].join();
    }

    std::cout << "All threads finished\n";
}