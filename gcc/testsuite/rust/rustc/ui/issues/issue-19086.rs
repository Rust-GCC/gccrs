use Foo::FooB;

enum Foo {
    FooB { x: i32, y: i32 }
}

fn main() {
    let f = FooB { x: 3, y: 4 };
    match f {
        FooB(a, b) => println!("{} {}", a, b),
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
}

