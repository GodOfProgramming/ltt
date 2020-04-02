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
  let contents = fs::read_to_string(args.filename)?;

  for line in search(&args.query, &contents) {
    println!("{}", line);
  }

  Ok(())
}

pub fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
  let mut results = Vec::new();

  for line in contents.lines() {
    if line.contains(query) {
      results.push(line);
    }
  }

  results
}

#[cfg(test)]
mod lib_tests;
