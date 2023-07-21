#include <cmath>
#include <random>
#include "threadPool.h"

int expo(int a, int b) {
    return pow(a, b);
}

int main(void) {

    unsigned int maxThreads = std::thread::hardware_concurrency();
    long int a = 0, b = 0;
	int numTasks = maxThreads * 5000;
    std::cout << "maxThreads is around: " << maxThreads << "\n";
	
    ThreadPool<long int> tPool(maxThreads);
    std::random_device r;
    std::default_random_engine dre(r());
    std::uniform_int_distribution<int> geny(0, 12);
    std::future<long int> results[numTasks];

    for (int i = 0; i < numTasks; ++i) {
        //std::cout << "i1: " << std::to_string(i) << "\n";
        a = geny(dre);
        b = geny(dre);
        std::packaged_task<long int()> task(std::bind(&expo, a, b));
        results[i] = tPool.pushTask(std::move(task));
    }

    tPool.waitAndShutdown();

    for (int i = 0; i < numTasks; ++i) {
        //std::cout << "i2: " << std::to_string(i);
        try {
            int exponent = results[i].get();
            std::cout << " Exponent: " << std::to_string(exponent) << "\n";
        }
        catch (const std::future_error& e) {
             std::cout << "Caught a future_error with code \"" << e.code()
                  << "\"\nMessage: \"" << e.what() << "\"\n";
        }
        
    }
}