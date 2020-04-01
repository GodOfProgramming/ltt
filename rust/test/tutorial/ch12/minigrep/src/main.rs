use std::env;
use std::process;

use minigrep;
use minigrep::Args;

fn main() {
    let args: Vec<String> = env::args().collect();

    let args = Args::new(&args).unwrap_or_else(|err| {
        println!("could not parse arguments! error: {}", err);
        process::exit(1);
    });

    if let Err(e) = minigrep::run(&args) {
      println!("Application error: {}", e);
      process::exit(1);
    }
}
