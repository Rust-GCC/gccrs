trait Trait {}

struct Foo<T:Trait> {
    x: T,
}

enum Bar<T:Trait> {
    ABar(isize),
    BBar(T),
    CBar(usize),
}

fn explode(x: Foo<u32>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn kaboom(y: Bar<f32>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
}

