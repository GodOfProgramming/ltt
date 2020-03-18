enum SpreadsheetCell {
  Int(i32),
  Float(f64),
  Text(String),
}

fn main() {
  let mut v = vec![1, 2, 3, 4, 5];

  for i in &mut v {
    *i += 50;
  }

  let row = vec![
    SpreadsheetCell::Int(3),
    SpreadsheetCell::Text(String::from("cell contents")),
    SpreadsheetCell::Float(10.12),
  ];

  for column in row {
    match column {
      SpreadsheetCell::Int(int) => println!("Integer: {}", int),
      SpreadsheetCell::Float(float) => println!("Float: {}", float),
      SpreadsheetCell::Text(string) => println!("String: {}", string),
    };
  }
}
