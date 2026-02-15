struct Foo(u8);

#[derive(Clone)]
struct FooHolster {
    the_foos: Vec<Foo>, // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {}

