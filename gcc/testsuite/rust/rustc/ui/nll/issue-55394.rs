struct Bar;

struct Foo<'s> {
    bar: &'s mut Bar,
}

impl Foo<'_> {
    fn new(bar: &mut Bar) -> Self {
        Foo { bar } // { dg-error ".E0495." "" { target *-*-* } }
    }
}

fn main() { }

