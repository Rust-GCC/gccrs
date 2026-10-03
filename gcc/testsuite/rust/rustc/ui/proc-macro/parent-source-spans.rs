// aux-build:parent-source-spans.rs

#![feature(decl_macro)]

extern crate parent_source_spans;

use parent_source_spans::parent_source_spans;

macro one($a:expr, $b:expr) {
    two!($a, $b);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

macro two($a:expr, $b:expr) {
    three!($a, $b);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
}

// forwarding tokens directly doesn't create a new source chain
macro three($($tokens:tt)*) {
    four!($($tokens)*);
}

macro four($($tokens:tt)*) {
    parent_source_spans!($($tokens)*);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
// { dg-error ".E0425." "" { target *-*-* } .-3 }
}

fn main() {
    one!("hello", "world");
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }

    two!("yay", "rust");
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }

    three!("hip", "hop");
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
}

