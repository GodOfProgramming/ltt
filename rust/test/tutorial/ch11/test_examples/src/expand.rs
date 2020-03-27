use crate::rect;

impl rect::Rect {
    pub fn expand(&mut self, scaler: u32) {
        self.width *= scaler;
        self.height *= scaler;
    }
}

