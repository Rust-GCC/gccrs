struct Foo {
    x: i32
}

impl *mut Foo {} // { dg-error ".E0390." "" { target *-*-* } }

fn main() {
}

