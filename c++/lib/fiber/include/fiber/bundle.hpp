#pragma once

#include <cinttypes>
#include <functional>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "fiber.hpp"

namespace fiber
{
  class Bundle
  {
   public:
    Bundle(uint8_t fiberCount = 1);
    ~Bundle();

    void push(std::function<void(void)> func);

   private:
    std::atomic<bool> mAlive;

    std::unique_ptr<std::thread> mMasterThread;
    std::deque<std::shared_ptr<Fiber>> mFibers;

    std::deque<std::function<void(void)>> mJobs;
    std::mutex mJobCountLock;

    std::mutex mNoJobsLock;
    std::condition_variable mNoJobsVar;

    uint8_t mFreeFibers;

    void notify();
  };

  Bundle::Bundle(uint8_t fiberCount): mFreeFibers(fiberCount)
  {
    for (uint8_t i = 0; i < fiberCount; i++) {
      auto fiber = std::make_shared<Fiber>();
      fiber->onFinish([this] {
        mJobCountLock.lock();
        this->mFreeFibers++;
        mJobCountLock.unlock();
        this->notify();
      });
      mFibers.push_back(fiber);
    }

    mMasterThread = std::make_unique<std::thread>([this]() {
      while (mAlive) {
        mJobCountLock.lock();  // prevent adding to the job list when checking
        if (mJobs.size() == 0) {
          std::unique_lock<std::mutex> lock(mNoJobsLock);
          mJobCountLock.unlock();  // allow jobs to be added
          mNoJobsVar.wait(lock);   // immediatly wait
        } else {
          mJobCountLock.unlock();  // allow jobs to be added
        }

        mJobCountLock.lock();
        {
          for (auto& fiber : mFibers) {
            // if there's no available resources, break
            if (mJobs.size() == 0 || mFreeFibers == 0 || !mAlive) {
              break;
            }

            if (!fiber->has_task()) {
              mFreeFibers--;
              fiber->assign(mJobs.front());
              fiber->run();
              mJobs.pop_front();
            }
          }
        }
        mJobCountLock.unlock();  // allow jobs to be added
      }
    });
  }

  Bundle::~Bundle()
  {
    mAlive = false;
    this->notify();
    this->mMasterThread->join();

    for (auto& fiber : mFibers) {
      fiber.reset();
    }
  }

  void Bundle::push(std::function<void(void)> job)
  {
    mJobCountLock.lock();
    mJobs.push_back(job);     // add a job to the queue
    mNoJobsVar.notify_one();  // wake up the master thread
    mJobCountLock.unlock();
  }

  void Bundle::notify()
  {
    mJobCountLock.lock();
    mNoJobsVar.notify_one();  // wake up the master thread
    mJobCountLock.unlock();
  }
}  // namespace fiber
