fn something<'a>(x: &'a i32, y: &'a i32) -> &'a i32 {
  if x > y {
    x
  } else {
    y
  }
}

fn main() {
  let x = 0;
  let y = 1;

  let z = something(&x, &y);

  println!("x = {}\ny = {}\nz = {}", x, y, z);
}
