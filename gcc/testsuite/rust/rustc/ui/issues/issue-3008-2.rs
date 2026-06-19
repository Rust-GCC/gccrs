enum Foo { Foo_(Bar) }
struct Bar { x: Bar }
// { dg-error ".E0072." "" { target *-*-* } .-1 }

fn main() {
}

