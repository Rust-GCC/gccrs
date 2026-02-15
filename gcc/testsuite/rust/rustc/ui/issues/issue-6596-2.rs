#![feature(macro_rules)]

macro_rules! g {
    ($inp:ident) => (
        { $inp $nonexistent }
// { dg-error "" "" { target *-*-* } .-1 }
    );
}

fn main() {
    let foo = 0;
    g!(foo);
}

