// ignore-tidy-linelength

#![deny(warnings)]

struct Borked {}

impl Borked {
    fn a(&self) {}
}

fn run_wild<T>(b: &Borked) {
    b.a::<'_, T>();
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-warning ".E0107." "" { target *-*-* } .-3 }
}

fn main() {}

