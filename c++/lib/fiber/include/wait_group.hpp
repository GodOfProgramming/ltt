#pragma once
#include <mutex>
#include <condition_variable>

namespace fiber
{
    class WaitGroup
    {
       public:
        WaitGroup(int signal_count);

        /* Signal to the wait group that a task is done */
        bool signal();

        /* Wait until the specified number of signals are reached */
        bool wait();

       private:
        const int mSignalCount;
        int mSignals;
        std::mutex mSignalLock;
        std::mutex mWaitLock;
        std::condition_variable mWaitVar;
    };

    inline WaitGroup::WaitGroup(int signal_count) : mSignalCount(signal_count), mSignals(0)
    {}

    inline bool WaitGroup::signal()
    {
        bool done = false;
        mSignalLock.lock();
        {
            mWaitLock.lock();
            {
                if (++mSignals == mSignalCount) {
                    mWaitVar.notify_one();
                    done = true;
                }
            }
            mWaitLock.unlock();
        }
        mSignalLock.unlock();
        return done;
    }

    inline bool WaitGroup::wait()
    {
        bool done = false;
        mSignalLock.lock();
        {
            if (mSignals >= mSignalCount) {
                done = true;
            }
        }
        mSignalLock.unlock();

        if (!done) {
            mWaitLock.lock();
            std::mutex wait_lock;
            std::unique_lock<std::mutex> lock(wait_lock);
            mWaitLock.unlock();
            mWaitVar.wait(lock);
        }

        return done;
    }
}  // namespace fiber

