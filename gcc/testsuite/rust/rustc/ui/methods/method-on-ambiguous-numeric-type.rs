// aux-build:macro-in-other-crate.rs

#[macro_use] extern crate macro_in_other_crate;

macro_rules! local_mac {
    ($ident:ident) => { let $ident = 42; }
}

fn main() {
    let x = 2.0.neg();
// { dg-error ".E0689." "" { target *-*-* } .-1 }

    let y = 2.0;
    let x = y.neg();
// { dg-error ".E0689." "" { target *-*-* } .-1 }
    println!("{:?}", x);

    for i in 0..100 {
        println!("{}", i.pow(2));
// { dg-error ".E0689." "" { target *-*-* } .-1 }
    }

    local_mac!(local_bar);
    local_bar.pow(2);
// { dg-error ".E0689." "" { target *-*-* } .-1 }
}

fn qux() {
    mac!(bar);
    bar.pow(2);
// { dg-error ".E0689." "" { target *-*-* } .-1 }
}

