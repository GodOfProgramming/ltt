pub struct Args<'a> {
  pub query: &'a String,
  pub filename: &'a String,
}

impl<'a> Args<'a> {
  pub fn new(args: &[String]) -> Result<Args, &'static str> {
    if args.len() > 2 {
      Ok(Args {
        query: &args[1],
        filename: &args[2],
      })
    } else {
      Err("need more arguments")
    }
  }
}
