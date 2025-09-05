trait Foo {
    const ID: i32;
}

const X: i32 = <i32>::ID;
// { dg-error ".E0599." "" { target *-*-* } .-1 }

fn main() {
    assert_eq!(1, X);
}

