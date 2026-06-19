// run-rustfix
// Issue #50974

pub struct Foo {
    pub a: u8,
    pub b: u8
}

fn main() {
    let _ = Foo {
        a: 0,,
// { dg-error "" "" { target *-*-* } .-1 }
        b: 42
    };
}

