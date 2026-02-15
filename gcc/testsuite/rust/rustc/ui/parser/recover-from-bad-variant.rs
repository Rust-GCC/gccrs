enum Enum {
    Foo { a: usize, b: usize },
    Bar(usize, usize),
}

fn main() {
    let x = Enum::Foo(a: 3, b: 4);
// { dg-error "" "" { target *-*-* } .-1 }
    match x {
        Enum::Foo(a, b) => {}
// { dg-error ".E0532." "" { target *-*-* } .-1 }
        Enum::Bar { a, b } => {}
// { dg-error ".E0769." "" { target *-*-* } .-1 }
    }
}

