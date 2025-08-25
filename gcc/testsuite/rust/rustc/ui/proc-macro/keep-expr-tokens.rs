// aux-build:test-macros.rs

#![feature(stmt_expr_attributes)]
#![feature(proc_macro_hygiene)]

extern crate test_macros;

use test_macros::recollect_attr;

fn main() {
    #[test_macros::recollect_attr]
    for item in missing_fn() {} // { dg-error ".E0425." "" { target *-*-* } }

    (#[recollect_attr] #[recollect_attr] ((#[recollect_attr] bad))); // { dg-error ".E0425." "" { target *-*-* } }
}

