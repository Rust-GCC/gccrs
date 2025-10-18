// Issue #50124 - Test warning for unused operator expressions

// check-pass

#![warn(unused_must_use)]

fn main() {
    let val = 1;
    let val_pointer = &val;

// Comparison Operators
    val == 1; // { dg-warning "" "" { target *-*-* } }
    val < 1; // { dg-warning "" "" { target *-*-* } }
    val <= 1; // { dg-warning "" "" { target *-*-* } }
    val != 1; // { dg-warning "" "" { target *-*-* } }
    val >= 1; // { dg-warning "" "" { target *-*-* } }
    val > 1; // { dg-warning "" "" { target *-*-* } }

// Arithmetic Operators
    val + 2; // { dg-warning "" "" { target *-*-* } }
    val - 2; // { dg-warning "" "" { target *-*-* } }
    val / 2; // { dg-warning "" "" { target *-*-* } }
    val * 2; // { dg-warning "" "" { target *-*-* } }
    val % 2; // { dg-warning "" "" { target *-*-* } }

// Logical Operators
    true && true; // { dg-warning "" "" { target *-*-* } }
    false || true; // { dg-warning "" "" { target *-*-* } }

// Bitwise Operators
    5 ^ val; // { dg-warning "" "" { target *-*-* } }
    5 & val; // { dg-warning "" "" { target *-*-* } }
    5 | val; // { dg-warning "" "" { target *-*-* } }
    5 << val; // { dg-warning "" "" { target *-*-* } }
    5 >> val; // { dg-warning "" "" { target *-*-* } }

// Unary Operators
    !val; // { dg-warning "" "" { target *-*-* } }
    -val; // { dg-warning "" "" { target *-*-* } }
    *val_pointer; // { dg-warning "" "" { target *-*-* } }
}

