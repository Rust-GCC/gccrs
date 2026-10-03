struct Foo(u8);

impl Foo {
    fn bar(&self) -> bool { self.0 > 5 }
    fn bar() {} // { dg-error ".E0201." "" { target *-*-* } }
}

trait Baz {
    type Quux;
    fn baz(&self) -> bool;
}

impl Baz for Foo {
    type Quux = u32;

    fn baz(&self) -> bool { true }
    fn baz(&self) -> bool { self.0 > 5 } // { dg-error ".E0201." "" { target *-*-* } }
    type Quux = u32; // { dg-error ".E0201." "" { target *-*-* } }
}

fn main() {
}

