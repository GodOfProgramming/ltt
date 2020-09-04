#pragma once

namespace util
{
  template <typename T>
  class Channel
  {
   public:
    virtual ~Channel() = default;

    void close();

    auto is_closed() -> bool;

    auto size() -> size_t;

   protected:
    Channel(
     std::shared_ptr<std::atomic<bool>> closed,
     std::shared_ptr<std::list<T>> queue,
     std::shared_ptr<std::mutex> lock,
     std::shared_ptr<std::condition_variable> var);

    std::shared_ptr<std::atomic<bool>> closed;
    std::shared_ptr<std::list<T>> queue;
    std::shared_ptr<std::mutex> lock;
    std::shared_ptr<std::condition_variable> condvar;
  };

  template <typename T>
  class Sender: public Channel<T>
  {
   public:
    Sender(
     std::shared_ptr<std::atomic<bool>> closed,
     std::shared_ptr<std::list<T>> queue,
     std::shared_ptr<std::mutex> lock,
     std::shared_ptr<std::condition_variable> var);
    ~Sender() override = default;

    void send(T& msg);
  };

  template <typename T>
  class Receiver: public Channel<T>
  {
   public:
    Receiver(
     std::shared_ptr<std::atomic<bool>> closed,
     std::shared_ptr<std::list<T>> queue,
     std::shared_ptr<std::mutex> lock,
     std::shared_ptr<std::condition_variable> var);
    ~Receiver() override = default;

    auto recv(T& msg) -> bool;

    auto ready() -> bool;

   private:
    std::shared_ptr<std::mutex> wait_lock;
  };

  template <typename T>
  inline std::tuple<Sender<T>, Receiver<T>> makeChannel()
  {
    auto closed = std::make_shared<std::atomic<bool>>(false);
    auto lock = std::make_shared<std::mutex>();
    auto queue = std::make_shared<std::list<T>>();
    auto var = std::make_shared<std::condition_variable>();

    Sender<T> sender(closed, queue, lock, var);
    Receiver<T> receiver(closed, queue, lock, var);

    return {sender, receiver};
  }

  template <typename T>
  Channel<T>::Channel(
   std::shared_ptr<std::atomic<bool>> closed,
   std::shared_ptr<std::list<T>> queue,
   std::shared_ptr<std::mutex> lock,
   std::shared_ptr<std::condition_variable> var)
   : closed(closed), queue(queue), lock(lock), condvar(var)
  {}

  template <typename T>
  Sender<T>::Sender(
   std::shared_ptr<std::atomic<bool>> closed,
   std::shared_ptr<std::list<T>> queue,
   std::shared_ptr<std::mutex> lock,
   std::shared_ptr<std::condition_variable> var)
   : Channel<T>(closed, queue, lock, var)
  {}

  template <typename T>
  Receiver<T>::Receiver(
   std::shared_ptr<std::atomic<bool>> closed,
   std::shared_ptr<std::list<T>> queue,
   std::shared_ptr<std::mutex> lock,
   std::shared_ptr<std::condition_variable> var)
   : Channel<T>(closed, queue, lock, var), wait_lock(std::make_shared<std::mutex>())
  {}

  template <typename T>
  void Channel<T>::close()
  {
    std::unique_lock<std::mutex> lock(*this->lock);
    *this->closed = true;
    condvar->notify_all();
  }

  template <typename T>
  auto Channel<T>::is_closed() -> bool
  {
    return *this->closed;
  }

  template <typename T>
  auto Channel<T>::size() -> size_t
  {
    return this->queue->size();
  }

  template <typename T>
  void Sender<T>::send(T& item)
  {
    std::unique_lock<std::mutex> lock(*this->lock);

    if (*this->closed) {
      LOG(DEBUG, "tried to send on a closed channel");
      return;
    }

    this->queue->push_back(std::move(item));

    this->condvar->notify_one();
  }

  template <typename T>
  auto Receiver<T>::recv(T& msg) -> bool
  {
    std::unique_lock<std::mutex> lock(*this->wait_lock);

    this->condvar->wait(lock, [this] {
      return *this->closed || !this->queue->empty();
    });

    // queue modification takes place within this block
    {
      std::unique_lock<std::mutex> lock(*this->mLock);

      if (this->queue->empty()) {
        return false;
      }

      msg = std::move(this->queue->front());

      this->queue->pop_front();

      return true;
    }
  }

  template <typename T>
  auto Receiver<T>::ready() -> bool
  {
    std::unique_lock<std::mutex>(*this->lock);
    return this->queue->size() > 0;
  }
}  // namespace util
