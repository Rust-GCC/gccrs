#![feature(const_generics)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn is_123<const N: usize>(x: [u32; N]) -> bool {
    match x {
        [1, 2, ..] => true, // { dg-error ".E0730." "" { target *-*-* } }
        _ => false
    }
}

fn main() {}

