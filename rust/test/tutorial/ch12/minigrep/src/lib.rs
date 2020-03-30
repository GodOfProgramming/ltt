pub struct Args {
  query: &String,
  filename: &String,
}

impl Args {
  pub fn new(args: &[String]) -> Args {
    Args {
      query: &args[1],
      filename: &args[2],
    }
  }
}
