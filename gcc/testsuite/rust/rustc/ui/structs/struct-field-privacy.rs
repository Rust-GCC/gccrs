// aux-build:struct_field_privacy.rs

extern crate struct_field_privacy as xc;

struct A {
    a: isize,
}

mod inner {
    pub struct A {
        a: isize,
        pub b: isize,
    }
    pub struct B {
        pub a: isize,
        b: isize,
    }
    pub struct Z(pub isize, isize);
}

fn test(a: A, b: inner::A, c: inner::B, d: xc::A, e: xc::B, z: inner::Z) {
    a.a;
    b.a; // { dg-error ".E0616." "" { target *-*-* } }
    b.b;
    c.a;
    c.b; // { dg-error ".E0616." "" { target *-*-* } }

    d.a; // { dg-error ".E0616." "" { target *-*-* } }
    d.b;

    e.a;
    e.b; // { dg-error ".E0616." "" { target *-*-* } }

    z.0;
    z.1; // { dg-error ".E0616." "" { target *-*-* } }
}

fn main() {}

