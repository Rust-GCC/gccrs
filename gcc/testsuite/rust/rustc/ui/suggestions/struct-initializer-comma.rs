// run-rustfix

pub struct Foo {
    pub first: bool,
    pub second: u8,
}

fn main() {
    let _ = Foo {
// { dg-error ".E0063." "" { target *-*-* } .-1 }
        first: true
        second: 25
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

