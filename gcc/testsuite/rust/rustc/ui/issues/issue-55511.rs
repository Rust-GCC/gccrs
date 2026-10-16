#![warn(indirect_structural_match)]
use std::cell::Cell;
trait Foo<'a> {
    const C: Option<Cell<&'a u32>>;
}

impl<'a, T> Foo<'a> for T {
    const C: Option<Cell<&'a u32>> = None;
}

fn main() {
    let a = 22;
    let b = Some(Cell::new(&a));
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    match b {
        <() as Foo<'static>>::C => { }
        _ => { }
    }
}

