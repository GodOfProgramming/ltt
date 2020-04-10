pub struct Point {
    x: i32,
    y: i32,
}

pub fn struct_destructuring() {
    let p = Point {
        x: 0,
        y: 7,
    };

    let Point { x: xpos, y: ypos } = p;

    assert_eq!(0, xpos);
    assert_eq!(7, ypos);

    // or
    
    let Point { x, y } = p;

    assert_eq!(0, x);
    assert_eq!(7, y);
}

pub fn struct_match() {
    let p = Point {
        x: 0,
        y: 7,
    };

    match p {
        Point { x, y: 0 } => println!("On the x axis at {}", x),
        Point { x: 0, y } => println!("On the y axis at {}", y),
        Point { x, y } => println!("On neither axis: ({}, {})", x, y),
    };
}

pub fn match_guards() {
    let num = Some(4);

    match num {
        Some(x) if x < 5 => println!("Less than 5: {}", x),
        Some(x) => println!("Greater than or equal to 5: {}", x),
        None => (),
    }
}

pub fn pipe_and_guard() {
    let x = 4;
    let y = false;

    match x {
        4 | 5 | 6 if y => println("yes"),
        _ => println!("no"),
    }
}

pub fn at_symbol() {
    enum Message {
        Hello { id: i32 },
    }

    let msg = Message::Hello { id: 5 };

    match msg {
        Message::Hello { id: id_variable @ 3..=7 } => {
            println!("Found an id in the range: {}", id_variable)
        },
        Message::Hello { id: 10..=12 } => {
            println!("Found an id in another range")
        },
        Message::Hello { id } => {
            println!("Found some other id")
        },
    }
}

