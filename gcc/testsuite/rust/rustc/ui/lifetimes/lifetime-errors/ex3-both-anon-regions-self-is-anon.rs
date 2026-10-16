struct Foo {
    field: i32,
}

impl Foo {
    fn foo<'a>(&self, x: &Foo) -> &Foo {
        if true { x } else { self } // { dg-error ".E0623." "" { target *-*-* } }
    }
}

fn main() {}

