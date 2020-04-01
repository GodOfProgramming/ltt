use std::error::Error;
use std::fs;

#[derive(Debug)]
pub struct Args<'a> {
  pub query: &'a String,
  pub filename: &'a String,
}

impl<'a> Args<'a> {
  pub fn new(args: &[String]) -> Result<Args, &'static str> {
    if args.len() > 2 {
      Ok(Args {
        query: &args[1],
        filename: &args[2],
      })
    } else {
      Err("need more arguments")
    }
  }
}

impl<'a> PartialEq for Args<'a> {
  fn eq(&self, other: &Args) -> bool {
    self.query == other.query && self.filename == other.filename
  }
}

pub fn run(args: &Args) -> Result<(), Box<dyn Error>> {
  println!("searching for {}", args.query);
  println!("in file {}", args.filename);

  let contents = fs::read_to_string(args.filename)?;

  println!("With text:\n{}", contents);

  Ok(())
}

#[cfg(test)]
mod lib_tests;
