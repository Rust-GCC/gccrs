// aux-build:multispan.rs

extern crate multispan;

use multispan::hello;

fn main() {
    // This one emits no error.
    hello!();

    // Exactly one 'hi'.
    hello!(hi); // { dg-error "" "" { target *-*-* } }

    // Now two, back to back.
    hello!(hi hi); // { dg-error "" "" { target *-*-* } }

    // Now three, back to back.
    hello!(hi hi hi); // { dg-error "" "" { target *-*-* } }

    // Now several, with spacing.
    hello!(hi hey hi yo hi beep beep hi hi); // { dg-error "" "" { target *-*-* } }
    hello!(hi there, hi how are you? hi... hi.); // { dg-error "" "" { target *-*-* } }
    hello!(whoah. hi di hi di ho); // { dg-error "" "" { target *-*-* } }
    hello!(hi good hi and good bye); // { dg-error "" "" { target *-*-* } }
}

