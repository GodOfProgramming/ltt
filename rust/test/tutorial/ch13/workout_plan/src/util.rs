pub struct Catcher<T>
where
  T: Fn(u32) -> u32,
{
  calculation: T,
  value: Option<u32>,
}

impl<T> Catcher<T>
where
  T: Fn(u32) -> u32,
{
  pub fn new(calculation: T) -> Catcher<T> {
    Catcher {
      calculation,
      value: None,
    }
  }

  pub fn value(&mut self, arg: u32) -> u32 {
    match self.value {
      Some(v) => v,
      None => {
        let v = (self.calculation)(arg);
        self.value = Some(v);
        v
      }
    }
  }
}
