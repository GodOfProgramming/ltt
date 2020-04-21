use std::net::UdpSocket;
use std::sync::mpsc;
use std::thread;
use byteorder::{BigEndian, ReadBytesExt};

struct Fragment {
  packet_type: u8,
  address: [u8; 19],
  timestamp: u64,
  mac: [u8; 32],
}

impl Fragment {
  fn new(packet_type: u8, address: [u8; 19], timestamp: u64, mac: [u8; 32]) -> Fragment {
    Fragment {
      packet_type,
      address,
      timestamp,
      mac,
    }
  }
}

fn recv(sender: mpsc::Sender<Vec<u8>>) -> std::io::Result<()> {
  let socket = UdpSocket::bind("127.0.0.1:51034")?;

  let mut buf = [0; 1500];
  while let Ok((amt, src)) = socket.recv_from(&mut buf) {
    let buf = &mut buf[..amt];
    let buf = Vec::from(buf);
    sender.send(buf).unwrap();
    socket.send_to("placeholder".as_bytes(), src).unwrap();
  }

  Ok(())
}

fn main() -> std::io::Result<()> {
  let (sender, receiver) = mpsc::channel::<Vec<u8>>();

  let t = thread::spawn(move || {
    let msg = receiver.recv().unwrap();

    let mut addr = [0; 19];
    addr.copy_from_slice(&msg[1..20]);

    let timestamp = (&msg[20..28]).read_u64::<BigEndian>().unwrap();

    let mut mac = [0; 32];
    mac.copy_from_slice(&msg[28..60]);

    let frag = Fragment::new(msg[0], addr, timestamp, mac);

    println!("Got fragment! Packet type: {}", frag.packet_type);
  });

  recv(sender)?;

  t.join().unwrap();

  Ok(())
}
