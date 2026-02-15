// run-rustfix

pub fn foo<T>(s: &[T], t: &[T]) {
    let _ = s == t; // { dg-error ".E0369." "" { target *-*-* } }
}

fn main() {}

