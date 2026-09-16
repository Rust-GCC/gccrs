#![feature(foo_bar_baz, foo(bar), foo = "baz", foo)]
// { dg-error ".E0556." "" { target *-*-* } .-1 }
// { dg-error ".E0556." "" { target *-*-* } .-2 }

#![feature] // { dg-error "" "" { target *-*-* } }
#![feature = "foo"] // { dg-error "" "" { target *-*-* } }

#![feature(test_removed_feature)] // { dg-error ".E0557." "" { target *-*-* } }

fn main() {}

