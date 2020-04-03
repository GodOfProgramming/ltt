use std::process;

fn fib(count: i32) -> i32 {
    let prev = 0;
    let curr = 1;

    let i = 0;
    while (i < count) {
        let tmp = curr;
        curr = curr + prev;
        prev = tmp;
        i += 1;
    }

    return prev;
}

fn main() {
    let args: Vec<String> = env::args().collect();
    
    if args.len() < 1 {
        println!("You need to supploy an argument");
        process::exit(1);
    }

    let count: i32 = args[1].trim().parse().expect("You need to use a number");

    if count < 0 {
        println!("You need to use a positive number");
        process::exit(1);
    }

    let result = fib(count);

    println("{}", result);
}

