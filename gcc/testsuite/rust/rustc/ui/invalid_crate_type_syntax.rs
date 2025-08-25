// regression test for issue 16974
#![crate_type(lib)]  // { dg-error "" "" { target *-*-* } }

fn my_lib_fn() {}

fn main() {}

