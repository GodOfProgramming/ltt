use std::env;
use std::error::Error;
use std::fs;

#[derive(Debug)]
pub struct Args<'a> {
  pub query: &'a String,
  pub filename: &'a String,
  pub case_sensitive: bool,
}

impl<'a> Args<'a> {
  pub fn new(args: &[String]) -> Result<Args, &'static str> {
    if args.len() > 2 {
      Ok(Args {
        query: &args[1],
        filename: &args[2],
        case_sensitive: env::var("CASE_INSENSITIVE").is_err(),
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

  let results = if args.case_sensitive {
    search(&args.query, &contents)
  } else {
    search_case_insensitive(&args.query, &contents)
  };

  for line in results {
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

pub fn search_case_insensitive<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
  let query = query.to_lowercase();
  let mut results = Vec::new();

  for line in contents.lines() {
    if line.to_lowercase().contains(&query) {
      results.push(line);
    }
  }

  results
}

#[cfg(test)]
mod lib_tests;
