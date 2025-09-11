trait I {}
type K = dyn I + 'static;

fn foo(_x: K) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {}

