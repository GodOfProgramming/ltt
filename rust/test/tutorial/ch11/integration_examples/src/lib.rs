pub mod adder;


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_adds_two() {
        assert_eq!(4, adder::add_two(2));
    }
}
