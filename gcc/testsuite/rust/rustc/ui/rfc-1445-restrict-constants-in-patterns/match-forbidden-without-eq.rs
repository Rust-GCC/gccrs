use std::f32;

#[derive(PartialEq)]
struct Foo {
    x: u32
}

const FOO: Foo = Foo { x: 0 };

fn main() {
    let y = Foo { x: 1 };
    match y {
        FOO => { }
// { dg-error "" "" { target *-*-* } .-1 }
        _ => { }
    }

    let x = 0.0;
    match x {
        f32::INFINITY => { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
        _ => { }
    }
}

