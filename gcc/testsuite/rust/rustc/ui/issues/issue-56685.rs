#![allow(dead_code)]
#![deny(unused_variables)]

// This test aims to check that unused variable suggestions update bindings in all
// match arms.

fn main() {
    enum E {
        A(i32,),
        B(i32,),
    }

    match E::A(1) {
        E::A(x) | E::B(x) => {}
// { dg-error "" "" { target *-*-* } .-1 }
    }

    enum F {
        A(i32, i32,),
        B(i32, i32,),
        C(i32, i32,),
    }

    let _ = match F::A(1, 2) {
        F::A(x, y) | F::B(x, y) => { y },
// { dg-error "" "" { target *-*-* } .-1 }
        F::C(a, b) => { 3 }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    };

    let _ = if let F::A(x, y) | F::B(x, y) = F::A(1, 2) {
// { dg-error "" "" { target *-*-* } .-1 }
        y
    } else {
        3
    };

    while let F::A(x, y) | F::B(x, y) = F::A(1, 2) {
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = y;
        break;
    }
}

