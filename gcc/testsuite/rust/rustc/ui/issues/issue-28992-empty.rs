// Can't use constants as tuple struct patterns


const C1: i32 = 0;

struct S;

impl S {
    const C2: i32 = 0;
}

fn main() {
    if let C1(..) = 0 {} // { dg-error ".E0532." "" { target *-*-* } }
    if let S::C2(..) = 0 {}
// { dg-error ".E0164." "" { target *-*-* } .-1 }
}

