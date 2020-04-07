use dereference::hello;
use dereference::MyBox;

fn main() {
  let x = 5;
  let y = Box::new(x);

  assert_eq!(5, x);
  assert_eq!(5, *y);

  let y = MyBox::new(x);

  assert_eq!(5, x);
  assert_eq!(5, *y);

  let m = MyBox::new(String::from("Rust"));
  hello(&m);
}
