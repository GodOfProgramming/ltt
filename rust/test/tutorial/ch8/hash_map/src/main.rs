use std::collections::HashMap;

fn main() {
  let mut scores = HashMap::new();

  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);

  // or

  let teams = vec![String::from("Blue"), String::from("Yellow")];
  let initial_scores = vec![10, 50];

  // example of a map from a tuple vector
  {
    let scores: HashMap<_, _> = teams.iter().zip(initial_scores.iter()).collect();

    let team_name = "Blue".to_string();
    let score = match scores.get(&team_name) {
      Some(num) => **num,
      None => 0,
    };
  }

  // overwrite value

  scores.insert(String::from("Blue"), 50);

  // insert only if no value

  scores.entry(String::from("Purple")).or_insert(50);

  // updating a value based on the old value

  let text = "hello world wonderful world";

  let mut map = HashMap::new();

  for word in text.split_whitespace() {
    let count = map.entry(word).or_insert(0);
    *count += 1;
  }
}
