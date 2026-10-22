trait Foo {
    type Item;
}

struct X;

impl Foo for X {
    type Item = bool;
}

fn print_x(_: &dyn Foo<Item=bool>, extra: &str) {
    println!("{}", extra);
}

fn main() {
    print_x(X);
// { dg-error ".E0061." "" { target *-*-* } .-1 }
}

