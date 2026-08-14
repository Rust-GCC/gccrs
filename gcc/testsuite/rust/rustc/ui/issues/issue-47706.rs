pub struct Foo {
    foo: Option<i32>,
}

impl Foo {
    pub fn new(foo: Option<i32>, _: ()) -> Foo {
        Foo { foo }
    }

    pub fn map(self) -> Option<Foo> {
        self.foo.map(Foo::new)
    }
// { dg-error ".E0593." "" { target *-*-* } .-2 }
}

enum Qux {
    Bar(i32),
}

fn foo<F>(f: F)
where
    F: Fn(),
{
}

fn main() {
    foo(Qux::Bar);
}
// { dg-error ".E0593." "" { target *-*-* } .-2 }

