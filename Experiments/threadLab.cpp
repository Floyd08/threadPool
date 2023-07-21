#include <iostream>
#include <thread>

void hello(int count) {

    std::cout << "Hello world " << count << "\n"; 
}

int main(void) {

    int maxThreads = 5;
    std::thread threads[5];

    for (int i = 0; i < maxThreads; ++i) {
        threads[i] = std::thread(hello, i);
    }

    for (int i = 0; i < maxThreads; ++i) {
        threads[i].join();
    }

    std::cout << "All threads have finished running\n";

    return 0;
}