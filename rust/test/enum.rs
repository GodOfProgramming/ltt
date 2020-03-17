enum TestThing {
  Thing1 { x: i32, y: i32 },
}

fn get_str(thing: TestThing) -> TestThing {
  thing
}

fn main() {
  let thing = match get_str(TestThing::Thing1{ x: 1, y: 2 }) {
    TestThing::Thing1 => .x + obj.y,
  };
  println!("thing = {}", thing);
}