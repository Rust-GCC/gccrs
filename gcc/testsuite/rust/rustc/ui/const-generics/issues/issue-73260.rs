// compile-flags: -Zsave-analysis

#![feature(const_generics)]
#![allow(incomplete_features)]
struct Arr<const N: usize>
where Assert::<{N < usize::max_value() / 2}>: IsTrue, // { dg-error "" "" { target *-*-* } }
{
}

enum Assert<const CHECK: bool> {}

trait IsTrue {}

impl IsTrue for Assert<true> {}

fn main() {
    let x: Arr<{usize::max_value()}> = Arr {};
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

