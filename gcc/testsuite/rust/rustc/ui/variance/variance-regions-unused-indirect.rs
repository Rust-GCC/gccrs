// Test that disallow lifetime parameters that are unused.

enum Foo<'a> { // { dg-error ".E0392." "" { target *-*-* } }
    Foo1(Bar<'a>)
}

enum Bar<'a> { // { dg-error ".E0392." "" { target *-*-* } }
    Bar1(Foo<'a>)
}

fn main() {}

