use std::env;
use std::process;

use minigrep;
use minigrep::Config;

fn main() {
  let args = Config::new(env::args()).unwrap_or_else(|err| {
    eprintln!("could not parse arguments! error: {}", err);
    process::exit(1);
  });

  if let Err(e) = minigrep::run(&args) {
    eprintln!("Application error: {}", e);
    process::exit(1);
  }
}
