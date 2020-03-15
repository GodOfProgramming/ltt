use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut prev = 0;
    let mut curr = 1;
    let count: i32 = args[1].trim().parse().expect("NaN");

    let mut i = 0;
    while i < count {
        let tmp = curr;
        curr += prev;
        prev = tmp;
        i += 1;
    }

    println!("Fibonacci seq indx {} is {}", count, prev);
}
