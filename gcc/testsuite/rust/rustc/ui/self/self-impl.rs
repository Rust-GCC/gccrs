// Test that unsupported uses of `Self` in impls don't crash

struct Bar;

trait Foo {
    type Baz;
}

trait SuperFoo {
    type SuperBaz;
}

impl Foo for Bar {
    type Baz = bool;
}

impl SuperFoo for Bar {
    type SuperBaz = bool;
}

impl Bar {
    fn f() {
        let _: <Self>::Baz = true;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
        let _: Self::Baz = true;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
    }
}

fn main() {}

