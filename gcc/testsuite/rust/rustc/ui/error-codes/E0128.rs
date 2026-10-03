struct Foo<T=U, U=()> { // { dg-error ".E0128." "" { target *-*-* } }
    field1: T,
    field2: U,
}

fn main() {
}

