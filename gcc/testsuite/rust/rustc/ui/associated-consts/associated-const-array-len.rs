trait Foo {
    const ID: usize;
}

const X: [i32; <i32 as Foo>::ID] = [0, 1, 2];
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
    assert_eq!(1, X);
}

