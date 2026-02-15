use std::boxed::Box;

fn main() {
    let _boxed: Box<u32, _> = Box::new(10);
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

