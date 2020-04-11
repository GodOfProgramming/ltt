use std::sync::{mpsc, Arc, Mutex};
use std::thread::{self, JoinHandle};

type Job = Box<dyn FnOnce() + Send + 'static>;

pub enum ThreadPoolCreationError {
  ZeroThreads(&'static str),
}

enum Message {
  NewJob(Job),
  Terminate,
}

pub struct ThreadPool {
  workers: Vec<Worker>,
  sender: mpsc::Sender<Message>,
}

impl ThreadPool {
  /// Create a new Thread Pool
  ///
  /// The count is the number of threads in the pool.
  pub fn new(count: usize) -> Result<ThreadPool, ThreadPoolCreationError> {
    if count == 0 {
      Err(ThreadPoolCreationError::ZeroThreads(
        "Cannot create a thread pool with zero threads",
      ))
    } else {
      let (sender, receiver) = mpsc::channel();
      let receiver = Arc::new(Mutex::new(receiver));

      let mut workers = Vec::with_capacity(count);

      for id in 0..count {
        workers.push(Worker::new(id, Arc::clone(&receiver)));
      }

      Ok(ThreadPool { workers, sender })
    }
  }

  pub fn execute<T>(&self, f: T)
  where
    T: FnOnce() + Send + 'static,
  {
    let job = Box::new(f);

    self.sender.send(Message::NewJob(job)).unwrap();
  }
}

impl Drop for ThreadPool {
  fn drop(&mut self) {
    println!("Sending terminate to all threads");
    for _ in &mut self.workers {
      self.sender.send(Message::Terminate).unwrap();
    }

    for worker in &mut self.workers {
      println!("Shutting down worker {}", worker.id);

      if let Some(handle) = worker.handle.take() {
        handle.join().unwrap();
      }
    }
  }
}

struct Worker {
  id: usize,
  handle: Option<JoinHandle<()>>,
}

impl Worker {
  pub fn new(id: usize, receiver: Arc<Mutex<mpsc::Receiver<Message>>>) -> Worker {
    let handle = Some(thread::spawn(move || loop {
      let message = receiver.lock().unwrap().recv().unwrap();

      match message {
        Message::NewJob(job) => {
          println!("Worker {} got a job; executing.", id);

          job();
        },
        Message::Terminate => {
          println!("Worker {} was told to terminate", id);

          break;
        },
      }
    }));

    Worker { id, handle }
  }
}
