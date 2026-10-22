#![feature(stmt_expr_attributes)]

// Test that various placements of the inner attribute are parsed correctly,
// or not.

fn main() {
    let a = #![allow(warnings)] (1, 2);
// { dg-error "" "" { target *-*-* } .-1 }

    let b = (#![allow(warnings)] 1, 2);

    let c = {
        #![allow(warnings)]
        (#![allow(warnings)] 1, 2)
    };

    let d = {
        #![allow(warnings)]
        let e = (#![allow(warnings)] 1, 2);
        e
    };
}

