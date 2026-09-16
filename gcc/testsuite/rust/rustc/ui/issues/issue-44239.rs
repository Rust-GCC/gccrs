fn main() {
    let n = 0;

    struct Foo;
    impl Foo {
        const N: usize = n;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
    }
}

