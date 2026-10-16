// Tests the new destructor semantics.

use std::cell::RefCell;

fn main() {
    let b = {
        let a = Box::new(RefCell::new(4));
        *a.borrow() + 1
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    println!("{}", b);
}

