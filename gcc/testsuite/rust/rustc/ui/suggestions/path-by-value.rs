use std::path::Path;

fn f(p: Path) { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {}

