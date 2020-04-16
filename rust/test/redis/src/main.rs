use std::io::prelude::*;
use std::net::TcpStream;

macro_rules! count_expr {
    () => { 0 };
    ($_e: expr $(, $rest: expr)*) => { 1 + count_expr!($($rest),*) }
}

macro_rules! redis {
  ($cmd:expr $(, $args:expr)* ) => {
    {
      let mut s = format!("*{}\r\n${}\r\n{}\r\n",  1 + count_expr!($($args),*), $cmd.len(), $cmd);
      $(
        s = format!("{}${}\r\n{}\r\n", s, $args.len(), $args);
      )*
      s
    }
  };
}

fn main() -> std::io::Result<()> {
  let mut stream = TcpStream::connect("127.0.0.1:6379")?;
  let mut response: [u8; 128] = [0; 128];

  let command = redis!("SET", "foo", "bar");
  stream.write(&command.as_bytes())?;
  let size = stream.read(&mut response)?;
  let v = Vec::from(&response[0..size]);
  println!("response: {}", String::from_utf8(v).unwrap());

  let command = redis!("GET", "foo");
  stream.write(&command.as_bytes())?;
  let size = stream.read(&mut response)?;
  let v = Vec::from(&response[0..size]);
  println!("response: {}", String::from_utf8(v).unwrap());

  Ok(())
}
