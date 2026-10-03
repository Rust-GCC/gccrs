#![feature(nll)]

fn main() {
    let i = &3;

    let f = |x: &i32| -> &i32 { x };
// { dg-error "" "" { target *-*-* } .-1 }
    let j = f(i);

    let g = |x: &i32| { x };
// { dg-error "" "" { target *-*-* } .-1 }
    let k = g(i);
}

