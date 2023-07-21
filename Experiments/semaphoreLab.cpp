#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>
#include <semaphore>

#define		pause	    3000


std::condition_variable aCV;
std::condition_variable doopCV;
std::mutex mut, but;
std::counting_semaphore<12> endSig(12);
int threadsRunning = 0;

void hello(int count) {
    ++threadsRunning;
    endSig.acquire();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "Hello world " << count << "\n"; 
    endSig.release();
    --threadsRunning;
}

int main(void) {

    unsigned int maxThreads = std::thread::hardware_concurrency();
    std::cout << "maxThreads is around: " << maxThreads << "\n";
    std::thread threads[maxThreads];

    for (int i = 0; i < maxThreads; ++i) {
        threads[i] = std::thread(hello, i);
    }

    std::cout << "Waiting on threads\n";
    while(true) {
        std::cout << "threads running: " << std::to_string(threadsRunning) << "\n";
        endSig.acquire();
        if (threadsRunning == 0) {
            break;
        }
        else {
            continue;
        }
    }

    for (int i = 0; i < maxThreads; ++i) {
        threads[i].join();
    }

    std::cout << "All threads have finished running\n";

    return 0;
}

