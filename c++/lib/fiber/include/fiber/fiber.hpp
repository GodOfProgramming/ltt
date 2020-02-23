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
    virtual ~Fiber();

    void assign(std::function<void(void)> func);

    void run();

    void wait();

    bool isAlive();

    bool hasTask();

    void onFinish(std::function<void(void)> onfinish);

   private:
    std::atomic<bool> mAlive;
    std::atomic<bool> mHasJob;

		std::unique_ptr<std::thread> mThread = nullptr;

    std::mutex mJobAssignLock;

    std::mutex mJobLock;
    std::condition_variable mJobVar;

    std::mutex mWaitLock;
    std::condition_variable mWaitVar;

    std::function<void(void)> mJob;

    std::function<void(void)> mOnFinish;

    void notifyJob();

    void notifyWait();

    void notify();

    void join();

    void free();
  };

  inline Fiber::Fiber()
  {
    mAlive = true;
    mHasJob = false;

    mThread = std::make_unique<std::thread>([this]() -> void {
      while (mAlive) {
        // If there is no job present, wait
        std::unique_lock<std::mutex> lock(mJobLock);
        mJobVar.wait(lock, [this]() -> bool {
          return mHasJob || !mAlive;
        });

        // when there is a job present, execute
        {
          std::lock_guard<std::mutex> lk(mJobAssignLock);
          if (mHasJob) {
            mJob();
            if (mOnFinish) {
              mOnFinish();
            }
            mHasJob = false;
            notifyWait();
          }
        }
      }
    });
  }

  inline Fiber::~Fiber()
  {
    // free the thread (mark it as dead and execute)
    free();

    // wait for it to finish
    join();
  }

  inline void Fiber::assign(std::function<void(void)> func)
  {
    // lock so if there is a running job it waits
    std::lock_guard<std::mutex> lk(mJobAssignLock);
    // assign the job
    mJob = func;
  }

  inline void Fiber::run()
  {
    // state there is a task
    mHasJob = true;

    // signal there is a job waiting
    notifyJob();
  }

  inline void Fiber::wait()
  {
    std::unique_lock<std::mutex> lock(mWaitLock);
    mWaitVar.wait(lock, [this]() -> bool {
      return !mHasJob && mAlive;
    });
  }

  inline bool Fiber::isAlive()
  {
    return mAlive;
  }

  inline void Fiber::notifyJob()
  {
    mJobVar.notify_one();
  }

  inline void Fiber::notifyWait()
  {
    mWaitVar.notify_one();
  }

  inline void Fiber::notify()
  {
    notifyJob();
    notifyWait();
  }

  inline void Fiber::join()
  {
    if (mThread != nullptr) {
      mThread->join();
    }
  }

  inline bool Fiber::hasTask()
  {
    return mHasJob;
  }

  inline void Fiber::onFinish(std::function<void(void)> onfinish)
  {
    mOnFinish = onfinish;
  }

  inline void Fiber::free()
  {
    // prevent the while loop from executing
    mAlive = false;

    // activate the condition variables so the program resumes
    notify();
  }
}  // namespace fiber

