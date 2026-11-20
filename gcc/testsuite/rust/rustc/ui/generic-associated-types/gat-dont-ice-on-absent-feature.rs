// rust-lang/rust#60654: Do not ICE on an attempt to use GATs that is
// missing the feature gate.

struct Foo;

impl Iterator for Foo {
    type Item<'b> = &'b Foo;
// { dg-error ".E0195." "" { target *-*-* } .-1 }
// { dg-error ".E0195." "" { target *-*-* } .-2 }

    fn next(&mut self) -> Option<Self::Item> {
        None
    }
}

fn main() { }

