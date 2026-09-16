// aux-build:attr-stmt-expr.rs

#![feature(proc_macro_hygiene)]

extern crate attr_stmt_expr;
use attr_stmt_expr::{expect_let, expect_print_stmt, expect_expr, expect_print_expr};

fn print_str(string: &'static str) {
    // macros are handled a bit differently
    #[expect_print_expr]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { help ".E0658." "" { target *-*-* } .-2 }
    println!("{}", string)
}

fn main() {
    #[expect_let]
    let string = "Hello, world!";

    #[expect_print_stmt]
    println!("{}", string);

    #[expect_expr]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { help ".E0658." "" { target *-*-* } .-2 }
    print_str("string")
}

