enum Foo {
    enum Bar { Baz }, // { dg-error "" "" { target *-*-* } }
    struct Quux { field: u8 }, // { dg-error "" "" { target *-*-* } }
    union Wibble { field: u8 }, // { dg-error "" "" { target *-*-* } }
    Bat,
}

fn main() { }

