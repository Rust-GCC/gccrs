// Point at correct span for self type

struct SomeType {}

trait Foo {
    fn handler(self: &SomeType); // { dg-error ".E0307." "" { target *-*-* } }
}

fn main() {}

