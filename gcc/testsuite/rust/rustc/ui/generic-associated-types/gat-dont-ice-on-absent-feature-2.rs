// rust-lang/rust#60654: Do not ICE on an attempt to use GATs that is
// missing the feature gate.

struct Foo;

trait MyTrait {
    type Item<T>;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

impl MyTrait for Foo {
    type Item<T> = T;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() { }

