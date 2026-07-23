// edition:2018

#![allow(non_camel_case_types)]

enum Foo {}

struct std;

fn main() {
    enum Foo { A, B }
    use Foo::*;
// { dg-error ".E0659." "" { target *-*-* } .-1 }

    let _ = (A, B);

    fn std() {}
    enum std {}
    use std as foo;
// { dg-error ".E0659." "" { target *-*-* } .-1 }
// { dg-error ".E0659." "" { target *-*-* } .-2 }
}

