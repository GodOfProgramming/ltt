fn foo() -> (i32, i32, i32) {
    let tup: (i32, i32, i32) = (1, 2, 3);
    return tup;
}

fn main() {
    let (x, _, z) = foo();

    println!("x = {}", x);
    println!("z = {}", z);
}
