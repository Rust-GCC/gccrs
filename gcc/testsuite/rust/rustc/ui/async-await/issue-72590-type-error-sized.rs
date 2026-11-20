// Regression test for issue #72590
// Tests that we don't emit a spurious "size cannot be statically determined" error
// edition:2018

struct Foo {
    foo: Nonexistent, // { dg-error ".E0412." "" { target *-*-* } }
    other: str
}

struct Bar {
    test: Missing // { dg-error ".E0412." "" { target *-*-* } }
}

impl Foo {
    async fn frob(self) {} // { dg-error ".E0277." "" { target *-*-* } }
}

impl Bar {
    async fn myfn(self) {}
}

fn main() {}

