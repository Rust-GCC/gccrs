enum Foo {
    #[inline]
// { dg-error ".E0518." "" { target *-*-* } .-1 }
    Variant,
}

fn main() {}

