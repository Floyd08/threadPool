/**
 * Copyright (c) <2023> <Conrad Lautenschlager>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/


#include <iostream>
#include <thread>
#include <queue>
#include <chrono>
#include <future>
#include <functional>

template <typename T>
class ThreadPool {
    public:
		/**
		 * Pushes a packaged_task with arguments already bound, with a return type of T
		*/
        std::future<T> pushTask(std::packaged_task<T()> task) {
            tQueue.qMutex.lock();

            std::future<T> result = task.get_future();
            tQueue.q.push(std::move(task));

            tQueue.qMutex.unlock();
            tQueue.newTasks.notify_one();

            return result;
        }

		/**
		 * Each thread is really just this function, running in an infinity loop and pulling from the queue until it's told to stop
		 */
        void worker() {
            while(keepAlive == true) {
				
				std::unique_lock<std::mutex> qLock(tQueue.qMutex);
				tQueue.newTasks.wait(qLock);

                if (!tQueue.q.empty()) {
                	++threadsBusy;
                	std::packaged_task<T()> task;
                	task = std::move(tQueue.q.front());
                	tQueue.q.pop();
					qLock.unlock();
                	task();
                	--threadsBusy;
                	threadFinished.notify_one();
                }
            }                              
        }

		/**
		 * creates a number of threads as specifed by the client
		 * keepAlive must be true or the worker threads will terminate immediately
		*/
        void startPool() {
            keepAlive = true;
            for(int i = 0; i < maxThreads; ++i) {
                threads.push_back(std::thread(&ThreadPool::worker, this));
            }
        }
		
		/**
		 * Executes an orderly shutdown of the threadpool, waiting for all tasks in the queue to be completed
		*/
        void waitAndShutdown() {
			while(shutdown != true) {
                std::unique_lock<std::mutex> tLock(tMutex);
				threadFinished.wait_for(tLock, std::chrono::seconds(1));
				if (threadsBusy == 0 && tQueue.q.empty()) {
            		break;
        		}
        		else {
            		continue;
        		}
			}
			keepAlive = false;
            tQueue.newTasks.notify_all();
		}

		/**
		 * Joins all threads. Any incomplete tasks in the queue will not be completed
		*/
        void endPool() {
            for(int i = 0; i < maxThreads; ++i) {
                threads[i].join();
            }
        }

		/**
		 *	Creates a pool of numThreads threads
		*/
        ThreadPool(int numThreads) {
            maxThreads = numThreads;
			shutdown = false;
            startPool();
        }

		/**
		 * If the ThreadPool should fall out of scope unexpectedly, it will terminate without completing all tasks
		*/
		~ThreadPool() {
			keepAlive = false;
			endPool();
			shutdown = true;
			threadFinished.notify_one();
		}
	
    private:
        int maxThreads, threadsBusy;
        std::vector<std::thread> threads;
		std::mutex tMutex;
		std::condition_variable threadFinished;
        bool keepAlive, shutdown;

		/**
		 * q:			a queue to hold tasks
		 * qMutex:		controls access to the queue
		 * newTask:		wakes idle threads when tasks become available
		*/
        struct taskQueue{								
            std::queue<std::packaged_task<T()>> q;
            std::mutex qMutex;
            std::condition_variable newTasks;
        };

		struct taskQueue tQueue;     
};