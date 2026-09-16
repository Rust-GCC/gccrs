struct Foo {
    a: isize,
    b: isize,
}

type Bar = Box<Foo>;

fn want_foo(f: Foo) {}
fn have_bar(b: Bar) {
    want_foo(b); // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
}

fn main() {}

