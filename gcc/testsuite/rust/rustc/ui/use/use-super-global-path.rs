#![allow(unused)]

struct S;
struct Z;

mod foo {
    use ::super::{S, Z}; // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }

    pub fn g() {
        use ::super::main; // { dg-error ".E0433." "" { target *-*-* } }
        main(); // { dg-error ".E0425." "" { target *-*-* } }
    }
}

fn main() { foo::g(); }

