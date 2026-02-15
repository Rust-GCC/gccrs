// run-pass

#![warn(pointer_structural_match)]

type Func = fn(usize, usize) -> usize;

fn foo(a: usize, b: usize) -> usize { a + b }
fn bar(a: usize, b: usize) -> usize { a * b }
fn test(x: usize) -> Func {
    if x % 2 == 0 { foo }
    else { bar }
}

const FOO: Func = foo;
const BAR: Func = bar;

fn main() {
    match test(std::env::consts::ARCH.len()) {
        FOO => println!("foo"), // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
        BAR => println!("bar"), // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
        _ => unreachable!(),
    }
}

