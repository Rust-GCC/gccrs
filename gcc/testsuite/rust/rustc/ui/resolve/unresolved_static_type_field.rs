fn f(_: bool) {}

struct Foo {
    cx: bool,
}

impl Foo {
    fn bar() {
        f(cx);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {}

