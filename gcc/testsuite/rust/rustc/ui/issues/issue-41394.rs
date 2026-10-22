enum Foo {
    A = "" + 1
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

enum Bar {
    A = Foo::A as isize
}

fn main() {}

