pub struct ThreadPool;

impl ThreadPool {
  pub fn new(count: u32) -> ThreadPool {
    ThreadPool {}
  }

  pub fn execute<T>(&self, _: T)
  where
    T: Fn(),
  {
  }
}
