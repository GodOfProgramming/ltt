use std::env;
use std::fs;

use minigrep::Args;

fn main() {
    let args: Vec<String> = env::args().collect();

    let args = match Args::new(&args) {
      Ok(a) => a,
      Err(err_str) => {
        panic!("could not parse arguments! error: {}", err_str);
      },
    };

    println!("searching for {}", args.query);
    println!("in file {}", args.filename);

    let contents = fs::read_to_string(args.filename).expect("Something went wrong reading the file");

    println!("With text:\n{}", contents);
}
