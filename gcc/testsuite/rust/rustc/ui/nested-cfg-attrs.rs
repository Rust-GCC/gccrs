#[cfg_attr(all(), cfg_attr(all(), cfg(foo)))]
fn f() {}

fn main() { f() } // { dg-error ".E0425." "" { target *-*-* } }

