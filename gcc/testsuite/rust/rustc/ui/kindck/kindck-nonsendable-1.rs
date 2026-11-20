use std::rc::Rc;

fn foo(_x: Rc<usize>) {}

fn bar<F:FnOnce() + Send>(_: F) { }

fn main() {
    let x = Rc::new(3);
    bar(move|| foo(x));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

