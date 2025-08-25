//! Attributes producing expressions in invalid locations

// aux-build:attr-stmt-expr.rs

#![feature(proc_macro_hygiene)]
#![feature(stmt_expr_attributes)]

extern crate attr_stmt_expr;
use attr_stmt_expr::{duplicate, no_output};

fn main() {
    let _ = #[no_output] "Hello, world!";
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = #[duplicate] "Hello, world!";
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = {
        #[no_output]
        "Hello, world!"
    };

    let _ = {
        #[duplicate]
// { dg-error "" "" { target *-*-* } .-1 }
        "Hello, world!"
    };
}

