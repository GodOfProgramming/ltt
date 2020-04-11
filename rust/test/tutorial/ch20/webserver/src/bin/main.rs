use std::fs;
use std::io::prelude::*;
use std::net::{TcpListener, TcpStream};
use std::thread;
use std::time::Duration;
use webserver::ThreadPool;

fn main() {
  let listener = TcpListener::bind("127.0.0.1:7878").unwrap();
  let pool = ThreadPool::new(4);

  for stream in listener.incoming() {
    let stream = stream.unwrap();

    handle_connection(stream);
    pool.execute(|| {});
  }
}

fn handle_connection(mut stream: TcpStream) {
  let mut buffer = [0; 512];

  stream.read(&mut buffer).unwrap();

  let resp = if buffer.starts_with(get("/").as_bytes()) {
    let html = fs::read_to_string("public/index.html").unwrap();
    response(200, "OK", &html)
  } else if buffer.starts_with(get("/sleep").as_bytes()) {
    thread::sleep(Duration::from_secs(5));
    let html = fs::read_to_string("public/index.html").unwrap();
    response(200, "OK", &html)
  } else {
    let html = fs::read_to_string("public/404.html").unwrap();
    response(404, "NOT FOUND", &html)
  };

  stream.write(resp.as_bytes()).unwrap();
  stream.flush().unwrap();
}

fn get(path: &'static str) -> String {
  format!("GET {} HTTP/1.1\r\n", path)
}

fn response(code: u32, code_desc: &'static str, body: &String) -> String {
  format!("HTTP/1.1 {} {}\r\n\r\n{}", code, code_desc, body)
}
