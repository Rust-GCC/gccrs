#[repr(C, u8)] // { dg-error ".E0566." "" { target *-*-* } }
// { dg-warning ".E0566." "" { target *-*-* } .-1 }
enum Foo {
    A,
    B,
}

#[repr(C)] // { dg-error ".E0566." "" { target *-*-* } }
// { dg-warning ".E0566." "" { target *-*-* } .-1 }
#[repr(u8)]
enum Bar {
    A,
    B,
}

fn main() {}

