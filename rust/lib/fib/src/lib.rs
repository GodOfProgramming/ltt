use std::env;
use std::process;

fn fib(count: i32) -> i32 {
  let mut prev = 0;
  let mut curr = 1;

  for _ in 0..count {
    let tmp = curr;
    curr += prev;
    prev = tmp;
  }

  return prev;
}

fn main() {
  let args: Vec<String> = env::args().collect();

  if args.len() < 1 {
    println!("You need to supply an argument");
    process::exit(1);
  }

  let count: i32 = args[1].trim().parse().expect("You need to use a number");

  if count < 0 {
    println!("You need to use a positive number");
    process::exit(1);
  }

  let result = fib(count);

  println!("{}", result);
}

#[cfg(test)]
mod lib_tests;
