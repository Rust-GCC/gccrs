// Test that attempts to construct infinite types via impl trait fail
// in a graceful way.
//
// Regression test for #38064.

trait Quux {}

fn foo() -> impl Quux { // { dg-error ".E0720." "" { target *-*-* } }
    struct Foo<T>(T);
    impl<T> Quux for Foo<T> {}
    Foo(bar())
}

fn bar() -> impl Quux { // { dg-error ".E0720." "" { target *-*-* } }
    struct Bar<T>(T);
    impl<T> Quux for Bar<T> {}
    Bar(foo())
}

// effectively:
//     struct Foo(Bar);
//     struct Bar(Foo);
// should produce an error about infinite size

fn main() { foo(); }

