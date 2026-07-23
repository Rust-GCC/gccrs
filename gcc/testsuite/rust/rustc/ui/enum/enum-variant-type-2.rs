// Test that enum variants are not actually types.

enum Foo {
    Bar
}

fn foo(x: Foo::Bar) {} // { dg-error ".E0573." "" { target *-*-* } }

fn main() {}

