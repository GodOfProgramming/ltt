use std::rc::{Rc, Weak};
use std::cell::RefCell;

#[derive(Debug)]
pub struct Node {
    pub value: i32,
    pub parent: RefCell<Weak<Node>>,
    pub children: RefCell<Vec<Rc<Node>>>,
}
