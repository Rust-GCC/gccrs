// build-fail

#![warn(const_err)]

const fn foo(x: u32) -> u32 {
    x
}

fn main() {
    const X: u32 = 0 - 1;
// { dg-warning "" "" { target *-*-* } .-1 }
    const Y: u32 = foo(0 - 1);
// { dg-warning "" "" { target *-*-* } .-1 }
    println!("{} {}", X, Y);
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
// { dg-warning ".E0080." "" { target *-*-* } .-3 }
// { dg-warning ".E0080." "" { target *-*-* } .-4 }
}

