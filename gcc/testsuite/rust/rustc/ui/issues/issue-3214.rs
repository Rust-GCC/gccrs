// ignore-tidy-linelength

fn foo<T>() {
    struct Foo {
        x: T, // { dg-error ".E0401." "" { target *-*-* } }
    }

    impl<T> Drop for Foo<T> {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
// { dg-error ".E0207." "" { target *-*-* } .-2 }
        fn drop(&mut self) {}
    }
}
fn main() { }

