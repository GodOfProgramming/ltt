use super::*;

#[test]
fn fib_0() {
  assert_eq!(fib(0), 0);
}

#[test]
fn fib_1() {
  assert_eq!(fib(1), 1);
}

#[test]
fn fib_2() {
  assert_eq!(fib(2), 1);
}

#[test]
fn fib_10() {
  assert_eq!(fib(10), 55);
}
