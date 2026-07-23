enum E {
    Foo,
    Bar(String)
}

struct S {
    x: E
}

fn f(x: String) {}

fn main() {
    let s = S { x: E::Bar("hello".to_string()) };
    match &s.x { // { dg-error ".E0507." "" { target *-*-* } }
        &E::Foo => {}
        &E::Bar(identifier) => f(identifier.clone())
    };
    match &s.x {
        &E::Foo => {}
        &E::Bar(ref identifier) => println!("{}", *identifier)
    };
}

