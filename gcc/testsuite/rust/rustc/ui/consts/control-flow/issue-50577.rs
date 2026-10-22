fn main() {
    enum Foo {
        Drop = assert_eq!(1, 1),
// { dg-error ".E0317." "" { target *-*-* } .-1 }
    }
}

