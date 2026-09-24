trait Foo {
    type Out: Sized;
}

impl Foo for String {
    type Out = String;
}

trait Bar: Foo {
    const FROM: Self::Out;
}

impl<T: Foo> Bar for T {
    const FROM: &'static str = "foo";
// { dg-error ".E0326." "" { target *-*-* } .-1 }
}

fn main() {}

