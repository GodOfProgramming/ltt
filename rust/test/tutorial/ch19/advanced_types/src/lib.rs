type Kilometers = i32;

pub fn print_kilos() {
    let x: i32 = 5;
    let y: Kilometers = 5;
    println!("x + y = {}", x + y);
}

pub type Thunk = Box<dyn Fn() + Send + 'static>;

pub fn use_thunk() {
    let f: Thunk = Box::new(|| println!("hi"));

    fn takes_long_type(f: Thunk) {
        f();
    }

    fn returns_long_type() -> Thunk {
        Box::new(||{})
    }

    takes_long_type(f);

    let f = returns_long_type();
}

pub fn never_return() -> ! {
    panic!("I don't return");
}

