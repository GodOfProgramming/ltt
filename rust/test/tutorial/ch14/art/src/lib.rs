//! # Art
//!
//! A library for modeling artistic concepts

pub use self::kinds::PrimaryColor;
pub use self::kinds::SecondaryColor;
pub use self::utils::mix;

pub mod kinds {
  /// The primary colors according to the RYB color model.
  pub enum PrimaryColor {
    Red,
    Yellow,
    Blue,
  }

  /// The secondary colors according to the RYB color model.
  pub enum SecondaryColor {
    Orange,
    Green,
    Purple,
  }
}

pub mod utils {
  use crate::kinds::*;
  pub fn mix(c1: PrimaryColor, c2: PrimaryColor) -> SecondaryColor {
    let chooser = |c| {
      match c {
        PrimaryColor::Red => 0,
        PrimaryColor::Yellow => 1,
        PrimaryColor::Blue => 2,
      }
    };

    let c1 = chooser(c1);
    let c2 = chooser(c2);

    match (c1 + c2) % 3 {
      0 => SecondaryColor::Orange,
      1 => SecondaryColor::Green,
      2 => SecondaryColor::Purple,
      _ => panic!("Logic error"),
    }
  }
}

#[cfg(test)]
mod tests {
  #[test]
  fn it_works() {
    assert_eq!(2 + 2, 4);
  }
}
