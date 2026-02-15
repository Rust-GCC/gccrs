struct A { pub i: isize }

pub trait E {
    fn foo(&self);
}

impl E for A {
    pub fn foo(&self) {} // { dg-error ".E0449." "" { target *-*-* } }
}

enum Foo {
    V1 { pub f: i32 }, // { dg-error ".E0449." "" { target *-*-* } }
    V2(pub i32), // { dg-error ".E0449." "" { target *-*-* } }
}

fn main() {}

