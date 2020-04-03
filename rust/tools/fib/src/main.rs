use std::env;
use std::process;
use fib;

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

  let result = fib::calculate(count);

  println!("{}", result);
}
