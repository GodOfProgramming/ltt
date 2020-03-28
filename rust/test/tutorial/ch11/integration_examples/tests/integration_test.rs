use integration_examples::adder;

mod common;

#[test]
fn it_adds_two() {
    let num = common::random_num();
    assert_eq!(num + 2, adder::add_two(num));
}
