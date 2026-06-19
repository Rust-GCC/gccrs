fn main () {
    struct Foo { a: bool };

    let f = Foo(); // { dg-error ".E0423." "" { target *-*-* } }
}

fn bar() {
    struct S { x: i32, y: i32 }
    #[derive(PartialEq)]
    struct T {}

    if let S { x: _x, y: 2 } = S { x: 1, y: 2 } { println!("Ok"); }
// { dg-error "" "" { target *-*-* } .-1 }
    if T {} == T {} { println!("Ok"); }
// { dg-error ".E0423." "" { target *-*-* } .-1 }
// { dg-error ".E0423." "" { target *-*-* } .-2 }
}

fn foo() {
    for _ in std::ops::Range { start: 0, end: 10 } {}
// { dg-error "" "" { target *-*-* } .-1 }
}

