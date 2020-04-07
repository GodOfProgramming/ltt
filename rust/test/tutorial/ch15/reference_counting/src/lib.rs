use std::rc::Rc;

pub enum List<T> {
    Cons(T, Rc<List<T>>),
    Nil,
}
