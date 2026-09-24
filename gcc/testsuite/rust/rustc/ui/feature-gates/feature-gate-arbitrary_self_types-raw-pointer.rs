struct Foo;

impl Foo {
    fn foo(self: *const Self) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

trait Bar {
    fn bar(self: *const Self);
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

impl Bar for () {
    fn bar(self: *const Self) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() {}

