#![forbid(non_upper_case_globals)]
#![allow(dead_code)]

static foo: isize = 1; // { dg-error "" "" { target *-*-* } }

static mut bar: isize = 1; // { dg-error "" "" { target *-*-* } }

#[no_mangle]
pub static extern_foo: isize = 1; // OK, because #[no_mangle] supersedes the warning

fn main() { }

