use std::rc::Rc;

pub fn main() {
    let _x = *Rc::new("hi".to_string());
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

