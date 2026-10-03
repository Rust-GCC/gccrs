use std::cell::Cell;

const fn foo1() {}
const fn foo2(x: i32) -> i32 { x }
const fn foo3() -> i32 { 42 }
const fn foo4() -> Cell<i32> { Cell::new(42) }
const fn foo5() -> Option<Cell<i32>> { Some(Cell::new(42)) }
const fn foo6() -> Option<Cell<i32>> { None }

fn main() {
    let x: &'static () = &foo1(); // { dg-error ".E0716." "" { target *-*-* } }
    let y: &'static i32 = &foo2(42); // { dg-error ".E0716." "" { target *-*-* } }
    let z: &'static i32 = &foo3(); // { dg-error ".E0716." "" { target *-*-* } }
    let a: &'static Cell<i32> = &foo4();  // { dg-error ".E0716." "" { target *-*-* } }
    let a: &'static Option<Cell<i32>> = &foo5(); // { dg-error ".E0716." "" { target *-*-* } }
    let a: &'static Option<Cell<i32>> = &foo6(); // { dg-error ".E0716." "" { target *-*-* } }
}

