use rand::Rng;

pub fn random_num() -> i32 {
    rand::thread_rng().gen_range(1, 100)
}
