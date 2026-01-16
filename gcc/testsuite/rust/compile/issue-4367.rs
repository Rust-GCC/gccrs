enum Foo {
    // { dg-warning "enum is never used" "" { target *-*-* } .-1 }
    A,
    B,
}

enum _Bar {
    A,
    B,
}
