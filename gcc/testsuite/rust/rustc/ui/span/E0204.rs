struct Foo {
    foo: Vec<u32>,
}

impl Copy for Foo { } // { dg-error ".E0204." "" { target *-*-* } }

#[derive(Copy)] // { dg-error ".E0204." "" { target *-*-* } }
struct Foo2<'a> {
    ty: &'a mut bool,
}

enum EFoo {
    Bar { x: Vec<u32> },
    Baz,
}

impl Copy for EFoo { } // { dg-error ".E0204." "" { target *-*-* } }

#[derive(Copy)] // { dg-error ".E0204." "" { target *-*-* } }
enum EFoo2<'a> {
    Bar(&'a mut bool),
    Baz,
}

fn main() {
}

