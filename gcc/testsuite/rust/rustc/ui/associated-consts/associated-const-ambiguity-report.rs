trait Foo {
    const ID: i32;
}

trait Bar {
    const ID: i32;
}

impl Foo for i32 {
    const ID: i32 = 1;
}

impl Bar for i32 {
    const ID: i32 = 3;
}

const X: i32 = <i32>::ID; // { dg-error ".E0034." "" { target *-*-* } }

fn main() {
    assert_eq!(1, X);
}

