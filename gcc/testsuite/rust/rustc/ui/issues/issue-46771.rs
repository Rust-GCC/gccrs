fn main() {
    struct Foo;
    (1 .. 2).find(|_| Foo(0) == 0); // { dg-error ".E0618." "" { target *-*-* } }
}

