use std::cmp;
use std::env;
use std::process;

#[derive(Debug)]
struct Rect {
    width: u32,
    height: u32,
}

impl Rect {
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn can_hold(&self, other: &Rect) -> bool {
        let flipped = Rect {
            width: self.height,
            height: self.width,
        };

        return (self.width > other.width && self.height > other.height)
            || (flipped.width > other.width && flipped.height > other.height);
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 3 {
        println!("Need more args");
        process::exit(1);
    }

    let x = &args[1];
    let y = &args[2];

    let x: u32 = x.trim().parse().expect("x NaN");
    let y: u32 = y.trim().parse().expect("x NaN");

    let rect = Rect {
        width: x,
        height: y,
    };

    println!("Your rect: {:#?}", rect);
    println!("The area is {}", rect.area());
}
