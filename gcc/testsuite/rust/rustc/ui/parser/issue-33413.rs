struct S;

impl S {
    fn f(*, a: u8) -> u8 {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {}

