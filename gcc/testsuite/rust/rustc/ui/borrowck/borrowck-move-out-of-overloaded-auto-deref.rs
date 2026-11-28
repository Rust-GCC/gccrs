use std::rc::Rc;

pub fn main() {
    let _x = Rc::new(vec![1, 2]).into_iter();
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

