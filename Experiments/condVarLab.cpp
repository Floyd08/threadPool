#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>

int pause = 3000;
std::condition_variable aCV;
std::condition_variable doopCV;
std::mutex mut, but;

void poop() {
    std::cout << "Poop";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    aCV.notify_one();
}

void a() {

    std::unique_lock<std::mutex> aLock(mut);
    aCV.wait(aLock);
    std::cout << " a ";
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    doopCV.notify_one();
}

void doop() {

    std::unique_lock<std::mutex> doopLock(but);
    doopCV.wait(doopLock);
    std::cout << "doop" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(pause));
}

int main(void) {

    std::cout << "Scoop a" << std::endl;
    std::thread t1(poop);
    std::thread t2(a);
    std::thread t3(doop);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Scoop" << std::endl;
}

