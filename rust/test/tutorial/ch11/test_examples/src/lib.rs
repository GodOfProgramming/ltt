pub mod expand;
pub mod rect;

pub fn greeting(name: &str) -> String {
    String::from("hello")
}

#[cfg(test)]
mod tests {
    use super::*;
    use maplit::hashmap;
    use std::collections::HashMap;

    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }

    #[test]
    fn success() {
        assert_ne!(1, 2);
    }

    #[test]
    fn map_fail() {
        let mut map1 = HashMap::new();
        map1.insert(2, 2);

        let mut map2 = HashMap::new();
        map2.insert(2, 2);

        let map3 = hashmap! {
            2 => 2,
        };

        assert_eq!(map1, map2);
        assert_eq!(map2, map3);
    }

    #[test]
    fn larget_rect_can_hold_smaller() {
        let larger = rect::Rect {
            width: 8,
            height: 7,
        };

        let smaller = rect::Rect {
            width: 5,
            height: 1,
        };

        assert!(larger.can_hold(&smaller));
    }

    #[test]
    fn smaller_cannot_hold_larger() {
        let larger = rect::Rect {
            width: 8,
            height: 7,
        };

        let smaller = rect::Rect {
            width: 5,
            height: 1,
        };

        assert!(!smaller.can_hold(&larger));
    }

    #[test]
    fn rect_expands() {
        let mut r = rect::Rect {
            width: 1,
            height: 1,
        };

        r.expand(2);

        assert_eq!(r.width, 2);
        assert_eq!(r.height, 2);
    }

    #[test]
    fn greeting_contains_name() {
        let result = greeting("Carol");
        assert!(
            result.contains("Carol"),
            "Greeting did not contain the name, value was '{}'",
            result
        );
    }
}
