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
        Fiber();
        ~Fiber();

        void assign(std::function<void(void)> func);

        void run();

        void wait();

        bool isAlive();

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

        void notify_job();

        void notify_wait();

        void notify();

        void join();

        bool has_task();

        void free();
    };

    inline Fiber::Fiber()
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

    inline Fiber::~Fiber()
    {
        // free the thread (mark it as dead and execute)
        free();

        // wait for it to finish
        join();

        // free memory
        delete mThread;
    }

    inline void Fiber::assign(std::function<void(void)> func)
    {
        // lock so if there is a running job it waits
        mJobAssignLock.lock();

        // assign the job
        mJob = func;

        // unlock so the task can run
        mJobAssignLock.unlock();
    }

    inline void Fiber::run()
    {
        // state there is a task
        mHasTask = true;

        // signal there is a job waiting
        notify_job();
    }

    inline void Fiber::wait()
    {
        if (mJob && mHasTask) {
            std::unique_lock<std::mutex> lock(mWaitLock);
            mWaitVar.wait(lock);
        }
    }

    inline bool Fiber::isAlive()
    {
        return mAlive;
    }

    inline void Fiber::notify_job()
    {
        mJobVar.notify_one();
    }

    inline void Fiber::notify_wait()
    {
        mWaitVar.notify_one();
    }

    inline void Fiber::notify()
    {
        notify_job();
        notify_wait();
    }

    inline void Fiber::join()
    {
        if (mThread != nullptr) {
            mThread->join();
        }
    }

    inline bool Fiber::has_task()
    {
        return mHasTask;
    }

    inline void Fiber::free()
    {
        // prevent the while loop from executing
        mAlive = false;

        // activate the condition variables so the program resumes
        notify_job();
        notify_wait();
    }
}  // namespace fiber

