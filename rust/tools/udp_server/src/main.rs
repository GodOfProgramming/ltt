use std::net::UdpSocket;
use std::str;

fn main() -> std::io::Result<()> {
  let socket = UdpSocket::bind("127.0.0.1:51034")?;

  let mut buf = [0; 1500];
  while let Ok((amt, src)) = socket.recv_from(&mut buf) {
    let buf = &mut buf[..amt];
    let data = str::from_utf8(&buf).unwrap();
    println!("data: {}", data);
  }

  Ok(())
}
