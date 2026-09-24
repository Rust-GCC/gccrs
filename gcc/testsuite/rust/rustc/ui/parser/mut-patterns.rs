// Can't put mut in non-ident pattern

// edition:2018

#![feature(box_patterns)]
#![allow(warnings)]

pub fn main() {
    let mut _ = 0; // { dg-error "" "" { target *-*-* } }
    let mut (_, _) = (0, 0); // { dg-error "" "" { target *-*-* } }

    let mut (x @ y) = 0; // { dg-error "" "" { target *-*-* } }

    let mut mut x = 0;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    struct Foo { x: isize }
    let mut Foo { x: x } = Foo { x: 3 };
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    let mut Foo { x } = Foo { x: 3 };
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    struct r#yield(u8, u8);
    let mut mut yield(become, await) = r#yield(0, 0);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }

    struct W<T, U>(T, U);
    struct B { f: Box<u8> }
    let mut W(mut a, W(b, W(ref c, W(d, B { box f }))))
// { dg-error "" "" { target *-*-* } .-1 }
        = W(0, W(1, W(2, W(3, B { f: Box::new(4u8) }))));

    // Make sure we don't accidentally allow `mut $p` where `$p:pat`.
    macro_rules! foo {
        ($p:pat) => {
            let mut $p = 0; // { dg-error "" "" { target *-*-* } }
        }
    }
    foo!(x);
}

