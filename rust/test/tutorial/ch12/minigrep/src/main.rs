use std::env;
use std::fs;

use minigrep::Args;

fn main() {
    let args: Vec<String> = env::args().collect();

    let query = &args[1];
    let filename = &args[2];

    let args = Args::new(&args);

    println!("searching for {}", query);
    println!("in file {}", filename);

    let contents = fs::read_to_string(filename).expect("Something went wrong reading the file");

    println!("With text:\n{}", contents);
}
