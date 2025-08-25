enum Foo {
    A = foo(), // { dg-error ".E0282." "" { target *-*-* } }
    B = foo(), // { dg-error ".E0282." "" { target *-*-* } }
}

const fn foo<T>() -> isize {
    0
}

fn main() {}

