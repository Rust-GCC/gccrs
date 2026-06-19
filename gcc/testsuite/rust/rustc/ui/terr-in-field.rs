struct Foo {
    a: isize,
    b: isize,
}

struct Bar {
    a: isize,
    b: usize,
}

fn want_foo(f: Foo) {}
fn have_bar(b: Bar) {
    want_foo(b); // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

