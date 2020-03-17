#[derive(Debug)]
enum UsState {
  Alabama,
  Alaska,
  NewYork,
}

enum Coin {
  Penny,
  Nickel,
  Dime,
  Quarter(UsState),
}

impl Coin {
  fn value(&self) -> u8 {
    match self {
      Coin::Penny => 1,
      Coin::Nickel => 5,
      Coin::Dime => 10,
      Coin::Quarter(state) => {
        println!("State quarter from {:?}", state);
        25
      }
    }
  }
}

fn main() {
  let wallet = vec![Coin::Dime, Coin::Nickel, Coin::Quarter(UsState::NewYork), Coin::Penny];

  let mut sum = 0;
  for coin in wallet {
    sum += coin.value();
  }

  println!("sum is {}", sum);
}
