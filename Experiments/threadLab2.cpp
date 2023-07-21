#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>

//using namespace std;

int pause = 10;

void hello(std::promise<std::string>&& promise, std::string greetings, int count) {
    
    std::string message = "";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    message = greetings + " world " + std::to_string(count) + "\n";
    promise.set_value(message);
    //return message; 
}

int main(void) {

    using std::thread;

    unsigned int maxThreads = thread::hardware_concurrency();
    std::cout << "maxThreads is around: " << maxThreads << "\n";
    std::string greetings = "Hello";
    //std::queue<thread> threads;               NO a queue of tasks, not threads
    std::thread threads[maxThreads];
    //std::promise<std::string> promises[maxThreads];
    std::future<std::string> results[maxThreads];

    for (int i = 0; i< maxThreads; ++i) {
        
        std::promise<std::string> promise;
        results[i] = promise.get_future();
        threads[i] = (thread(hello, std::move(promise), greetings, i));
    }

    for (int i = 0; i < maxThreads; ++i) {
        
        std::string message = results[i].get();
        std::cout << message;
        threads[i].join();
    }

    std::cout << "All threads finished\n";

}