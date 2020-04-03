pub fn fib(count: u128) -> u128 {
  let mut prev = 0;
  let mut curr = 1;

  for _ in 0..count {
    let tmp = curr;
    curr += prev;
    prev = tmp;
  }

  return prev;
}

#[cfg(test)]
mod lib_tests;
