// aux-build:subspan.rs

extern crate subspan;

use subspan::subspan;

// This one emits no error.
subspan!("");

// Exactly one 'hi'.
subspan!("hi"); // { dg-error "" "" { target *-*-* } }

// Now two, back to back.
subspan!("hihi"); // { dg-error "" "" { target *-*-* } }

// Now three, back to back.
subspan!("hihihi"); // { dg-error "" "" { target *-*-* } }

// Now several, with spacing.
subspan!("why I hide? hi!"); // { dg-error "" "" { target *-*-* } }
subspan!("hey, hi, hidy, hidy, hi hi"); // { dg-error "" "" { target *-*-* } }
subspan!("this is a hi, and this is another hi"); // { dg-error "" "" { target *-*-* } }
subspan!("how are you this evening"); // { dg-error "" "" { target *-*-* } }
subspan!("this is highly eradic"); // { dg-error "" "" { target *-*-* } }

fn main() { }

