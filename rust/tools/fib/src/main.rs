use std::env;
use std::process;
use sequences;

fn main() {
  let args: Vec<String> = env::args().collect();

  if args.len() < 1 {
    println!("You need to supply an argument");
    process::exit(1);
  }

  let count: u128 = args[1].trim().parse().expect("You need to use a positive number");

  let result = sequences::fib(count);

  println!("{}", result);
}
