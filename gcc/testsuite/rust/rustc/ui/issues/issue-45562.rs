// run-rustfix

#[no_mangle] pub const RAH: usize = 5;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

