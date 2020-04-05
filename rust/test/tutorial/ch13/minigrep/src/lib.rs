use std::env;
use std::error::Error;
use std::fs;

#[derive(Debug, PartialEq)]
pub struct Config {
  pub query: String,
  pub filename: String,
  pub case_sensitive: bool,
}

impl Config {
  pub fn new<T>(mut args: T) -> Result<Config, &'static str>
  where
    T: Iterator<Item = String>,
  {
    args.next();

    let query = match args.next() {
      Some(q) => q,
      None => return Err("Didn't get the query string"),
    };

    let filename = match args.next() {
      Some(f) => f,
      None => return Err("Didn't get a filename"),
    };

    let case_sensitive = env::var("CASE_INSENSITIVE").is_err();

    Ok(Config {
      query,
      filename,
      case_sensitive,
    })
  }
}

pub fn run(args: &Config) -> Result<(), Box<dyn Error>> {
  let contents = fs::read_to_string(&args.filename)?;

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
  contents.lines().filter(|l| l.contains(query)).collect()
}

pub fn search_case_insensitive<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
  let query = query.to_lowercase();
  contents
    .lines()
    .filter(|l| l.to_lowercase().contains(&query))
    .collect()
}

#[cfg(test)]
mod lib_tests;
