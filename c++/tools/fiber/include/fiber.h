#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace fiber
{
    class Fiber
    {
       public:
        inline Fiber()
        {
            mAlive = true;
            mHasTask = false;

            mThread = new std::thread([this]() -> void {
                while (mAlive) {
                    // If there is no job present, wait

                    if (!mJob || !mHasTask) {
                        std::unique_lock<std::mutex> lock(mJobLock);
                        mJobVar.wait(lock);
                    }

                    // when there is a job present, execute

                    mJobAssignLock.lock();

                    if (mJob && mHasTask) {
                        mJob();
                        mHasTask = false;
                        notify_wait();
                    }

                    mJobAssignLock.unlock();
                }
            });
        }

        inline ~Fiber()
        {
            // free the thread (mark it as dead and execute)
            free();

            // wait for it to finish
            join();

            // free memory
            delete mThread;
        }

        inline void assign(std::function<void(void)> func)
        {
            // lock so if there is a running job it waits
            mJobAssignLock.lock();

            // assign the job
            mJob = func;

            // unlock so the task can run
            mJobAssignLock.unlock();
        }

        inline void run()
        {
            // state there is a task
            mHasTask = true;

            // signal there is a job waiting
            notify_job();
        }

        inline void wait()
        {
            if (mJob && mHasTask) {
                std::unique_lock<std::mutex> lock(mWaitLock);
                mWaitVar.wait(lock);
            }
        }

        inline bool isAlive()
        {
            return mAlive;
        }

       private:
        std::thread* mThread = nullptr;

        std::mutex mJobAssignLock;

        std::mutex mJobLock;
        std::condition_variable mJobVar;

        std::mutex mWaitLock;
        std::condition_variable mWaitVar;

        std::atomic<bool> mAlive;
        std::atomic<bool> mHasTask;

        std::function<void(void)> mJob;

        inline void notify_job()
        {
            mJobVar.notify_one();
        }

        inline void notify_wait()
        {
            mWaitVar.notify_one();
        }

        inline void notify()
        {
            notify_job();
            notify_wait();
        }

        inline void join()
        {
            if (mThread != nullptr) {
                mThread->join();
            }
        }

        inline bool has_task()
        {
            return mHasTask;
        }

        inline void free()
        {
            // prevent the while loop from executing
            mAlive = false;

            // activate the condition variables so the program resumes
            notify_job();
            notify_wait();
        }
    };
}  // namespace fiber

