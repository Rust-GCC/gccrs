// Test that parentheses form doesn't work with struct types appearing in argument types.

struct Bar<A> {
    f: A
}

fn foo(b: Box<Bar()>) {
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
}

fn main() { }

