use std::io::prelude::*;
use std::net::TcpStream;
use redis::*;

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

    let command = redis!("SET", "num", "123");
    stream.write(&command.as_bytes())?;
    let size = stream.read(&mut response)?;
    let v = Vec::from(&response[0..size]);
    println!("response: {}", String::from_utf8(v).unwrap());

    let command = redis!("GET", "num");
    stream.write(&command.as_bytes())?;
    let size = stream.read(&mut response)?;
    let v = Vec::from(&response[0..size]);
    println!("response: {}", String::from_utf8(v).unwrap());

    let command = redis!("DBSIZE");
    stream.write(&command.as_bytes())?;
    let size = stream.read(&mut response)?;
    let v = Vec::from(&response[0..size]);
    println!("response: {}", String::from_utf8(v).unwrap());

    Ok(())
}
