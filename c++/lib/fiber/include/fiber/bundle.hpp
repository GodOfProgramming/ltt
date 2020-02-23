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

    void tie(std::function<void(void)> func);

   private:
    std::atomic<bool> mAlive;

    std::unique_ptr<std::thread> mDispatchThread;

    std::deque<std::shared_ptr<Fiber>> mFibers;
    std::atomic<uint8_t> mFreeFibers;

    std::deque<std::function<void(void)>> mJobs;
		std::atomic<uint8_t> mJobCount;

    std::mutex mTieLock;

    std::mutex mWaitLock;
    std::condition_variable mWaitVar;

    void notify();
  };

  Bundle::Bundle(uint8_t fiberCount): mFreeFibers(fiberCount), mJobCount(0)
  {
    for (uint8_t i = 0; i < fiberCount; i++) {
      auto fiber = std::make_shared<Fiber>();
      fiber->onFinish([this] {
        this->mFreeFibers++;
        this->notify();
      });
      mFibers.push_back(fiber);
    }

    mDispatchThread = std::make_unique<std::thread>([this]() {
      while (mAlive) {
        std::unique_lock<std::mutex> lock(mWaitLock);
        mWaitVar.wait(lock, [this]() -> bool {
          return (mFreeFibers > 0 && mJobCount > 0) || !mAlive;
        });

        {
          std::lock_guard<std::mutex> lk(mTieLock);
          for (auto& fiber : mFibers) {
            // if there's no available resources, break
            if (mJobCount == 0 || mFreeFibers == 0 || !mAlive) {
              break;
            }

            if (!fiber->hasTask()) {
              mFreeFibers--;
              fiber->assign(mJobs.front());
              fiber->run();
              mJobs.pop_front();
							mJobCount--;
            }
          }
        }
      }
    });
  }

  Bundle::~Bundle()
  {
    mAlive = false;
    this->notify();
    this->mDispatchThread->join();

    for (auto& fiber : mFibers) {
      fiber.reset();
    }
  }

  void Bundle::tie(std::function<void(void)> job)
  {
    std::lock_guard<std::mutex> lk(mTieLock);
    mJobs.push_back(job);  // add a job to the queue
		mJobCount++;
    notify();              // wake up the master thread
  }

  void Bundle::notify()
  {
    mWaitVar.notify_one();  // wake up the master thread
  }
}  // namespace fiber
