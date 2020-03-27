#[derive(Debug)]
pub struct Rect {
    pub width: u32,
    pub height: u32,
}

impl Rect {
    pub fn area(&self) -> u32 {
        self.width * self.height
    }

    pub fn can_hold(&self, other: &Rect) -> bool {
        let flipped = Rect {
            width: self.height,
            height: self.width,
        };

        return (self.width > other.width && self.height > other.height)
            || (flipped.width > other.width && flipped.height > other.height);
    }
}

