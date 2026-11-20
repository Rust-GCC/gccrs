use std::rc::Rc;

fn bar<T: Send>(_: T) {}

fn main() {
    let x = Rc::new(5);
    bar(x);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

