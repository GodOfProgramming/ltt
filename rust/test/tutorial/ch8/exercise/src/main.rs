pub mod mmm;

fn main() {
  let mut nums = vec![2, 5, 3, 2, 4];

  println!("Mean = {}", mmm::mean(&nums));

  nums.sort();

  println!("Median = {}", mmm::median(&nums));

  println!("Mode = {}", mmm::mode(&nums));
}
