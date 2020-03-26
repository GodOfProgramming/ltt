#[cfg(test)]
mod tests {
    use std::collections::HashMap;
    use maplit::hashmap;

    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }

    #[test]
    fn failure() {
        assert_eq!(1, 2);
    }

    #[test]
    fn map_fail() {
        let mut map1 = HashMap::new();
        map1.insert(1, 1);

        let mut map2 = HashMap::new();
        map2.insert(2, 2);

        let map3 = hashmap!{
            3 => 3,
        };

        assert_eq!(map1, map2);
        assert_eq!(map2, map3);
    }
}

