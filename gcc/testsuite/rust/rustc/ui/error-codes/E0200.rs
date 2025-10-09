struct Foo;

unsafe trait Bar { }

impl Bar for Foo { } // { dg-error ".E0200." "" { target *-*-* } }

fn main() {
}

