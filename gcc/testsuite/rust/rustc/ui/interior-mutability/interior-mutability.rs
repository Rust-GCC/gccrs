use std::cell::Cell;
use std::panic::catch_unwind;
fn main() {
    let mut x = Cell::new(22);
    catch_unwind(|| { x.set(23); });
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

