// Allows to track issue #75492:
// https://github.com/rust-lang/rust/issues/75492

use std::iter;

fn main() {
    format!("A number: {}". iter::once(42).next().unwrap());
// { dg-error "" "" { target *-*-* } .-1 }

    // Other kind of types are also checked:

    format!("A number: {}" / iter::once(42).next().unwrap());
// { dg-error "" "" { target *-*-* } .-1 }

    format!("A number: {}"; iter::once(42).next().unwrap());
// { dg-error "" "" { target *-*-* } .-1 }

    // Note: this character is an COMBINING COMMA BELOW unicode char
    format!("A number: {}" ̦ iter::once(42).next().unwrap());
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // Here recovery is tested.
    // If the `compile_error!` is emitted, then the parser is able to recover
    // from the incorrect first separator.
    format!("{}". compile_error!("fail"));
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

