// verify that after encountering a semicolon after an item the parser recovers
mod M {};
// { dg-error "" "" { target *-*-* } .-1 }
struct S {};
// { dg-error "" "" { target *-*-* } .-1 }
fn foo(a: usize) {};
// { dg-error "" "" { target *-*-* } .-1 }
fn main() {
    struct X {};  // ok
    let _: usize = S {};
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let _: usize = X {};
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    foo("");
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

