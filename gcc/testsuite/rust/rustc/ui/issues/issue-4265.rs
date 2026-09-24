struct Foo {
      baz: usize
}

impl Foo {
    fn bar() {
        Foo { baz: 0 }.bar();
    }

    fn bar() { // { dg-error ".E0201." "" { target *-*-* } }
    }
}

fn main() {}

