use std::time::Instant;

const REPS: usize = 10000000;
const LEN: usize = 1000;

fn main() {
    let mut z = vec![0; LEN];

    let before = Instant::now();
    
    for i in 0..REPS {
        let (mut x, mut y) = (0, 0);
        for j in 0..LEN {
          z[j] = x + y * i;
          x += 1;
          y += 1;
        }
    }

    let after = Instant::now();

    let diff = after - before;

    println!("nanos: {:.7}", diff.as_nanos() as f64 / REPS as f64);
}
