// issue #21405
struct Foo;

fn foo<F>(f: F) where F: FnMut(Foo) {}

fn main() {
    foo(|s| s.is_empty());
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

