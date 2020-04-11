use std::sync::{mpsc, Arc, Mutex};
use std::thread::{self, JoinHandle};

pub enum ThreadPoolCreationError {
  ZeroThreads(&'static str),
}

pub struct ThreadPool {
  threads: Vec<Worker>,
  sender: mpsc::Sender<Job>,
}

impl ThreadPool {
  /// Create a new Thread Pool
  ///
  /// The count is the number of threads in the pool.
  ///
  /// # Panics
  ///
  /// The 'new' function will panic if the count is zero.
  pub fn new(count: usize) -> Result<ThreadPool, ThreadPoolCreationError> {
    if count == 0 {
      Err(ThreadPoolCreationError::ZeroThreads(
        "Cannot create a thread pool with zero threads",
      ))
    } else {
      let (sender, receiver) = mpsc::channel();
      let receiver = Arc::new(Mutex::new(receiver));

      let mut threads = Vec::with_capacity(count);

      for id in 0..count {
        threads.push(Worker::new(id, Arc::clone(&receiver)));
      }

      Ok(ThreadPool { threads, sender })
    }
  }

  pub fn execute<T>(&self, f: T)
  where
    T: FnOnce() + Send + 'static,
  {
    let job = Box::new(f);

    self.sender.send(job).unwrap();
  }
}

struct Worker {
  id: usize,
  handle: JoinHandle<()>,
}

impl Worker {
  pub fn new(id: usize, receiver: Arc<Mutex<mpsc::Receiver<Job>>>) -> Worker {
    let handle = thread::spawn(move || loop {
      let job = receiver.lock().unwrap().recv().unwrap();

      println!("Worker {} got a job; executing.", id);

      job();
    });

    Worker { id, handle }
  }
}

type Job = Box<dyn FnOnce() + Send + 'static>;
