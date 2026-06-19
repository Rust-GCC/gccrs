#![feature(unboxed_closures)]

use std::ops::FnMut;

fn to_fn_mut<A,F:FnMut<A>>(f: F) -> F { f }

fn call_it<F:FnMut(isize,isize)->isize>(y: isize, mut f: F) -> isize {
// { dg-note "" "" { target *-*-* } .-1 }
    f(2, y)
}

pub fn main() {
    let f = to_fn_mut(|x: usize, y: isize| -> isize { (x as isize) + y });
// { dg-note "" "" { target *-*-* } .-1 }
    let z = call_it(3, f);
// { dg-error ".E0631." "" { target *-*-* } .-1 }
// { dg-note ".E0631." "" { target *-*-* } .-2 }
    println!("{}", z);
}

