use std::cell::Cell;
use std::rc::Rc;

// Fast path, main can see the concrete type returned.
fn before() -> impl Fn(i32) {
    let p = Rc::new(Cell::new(0));
    move |x| p.set(x)
}

fn send<T: Send>(_: T) {}

fn main() {
    send(before());
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    send(after());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

// Deferred path, main has to wait until typeck finishes,
// to check if the return type of after is Send.
fn after() -> impl Fn(i32) {
    let p = Rc::new(Cell::new(0));
    move |x| p.set(x)
}

