// Test that `Box` cannot be used with a lifetime argument.

struct Foo<'a> {
    x: Box<'a, isize> // { dg-error ".E0107." "" { target *-*-* } }
}

pub fn main() {
}

