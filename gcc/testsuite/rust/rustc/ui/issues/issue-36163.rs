const A: isize = Foo::B as isize;

enum Foo {
    B = A, // { dg-error ".E0391." "" { target *-*-* } }
}

fn main() {}

