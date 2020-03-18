pub fn mean(nums: &Vec<i32>) -> i32 {
  let mut total = 0;

  for num in nums {
    total += num;
  }

  if let 0 = nums.len() {
    0
  } else {
    total / nums.len() as i32
  }
}

pub fn median(nums: &Vec<i32>) -> i32 {
  nums[nums.len() / 2]
}

use std::collections::HashMap;
pub fn mode(nums: &Vec<i32>) -> i32 {
  let mut counters = HashMap::new();

  for num in nums.iter() {
    let count = counters.entry(*num).or_insert(0);
    *count += 1;
  }

  let mut result: (i32, i32) = (0, 0);
  for counter in counters {
    if result.1 < counter.1 {
      result = counter;
    }
  }

  return result.0;
}