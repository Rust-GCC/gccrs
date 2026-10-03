// aux-build:test-macros.rs
// gate-test-proc_macro_hygiene

#![feature(stmt_expr_attributes)]

#[macro_use]
extern crate test_macros;

fn _test_inner() {
    #![empty_attr] // { dg-error ".E0658." "" { target *-*-* } }
}

mod _test2_inner {
    #![empty_attr] // { dg-error ".E0658." "" { target *-*-* } }
}

#[empty_attr = "y"] // { dg-error "" "" { target *-*-* } }
fn _test3() {}

fn attrs() {
    // Statement, item
    #[empty_attr] // OK
    struct S;

    // Statement, macro
    #[empty_attr] // { dg-error ".E0658." "" { target *-*-* } }
    println!();

    // Statement, semi
    #[empty_attr] // { dg-error ".E0658." "" { target *-*-* } }
    S;

    // Statement, local
    #[empty_attr] // { dg-error ".E0658." "" { target *-*-* } }
    let _x = 2;

    // Expr
    let _x = #[identity_attr] 2; // { dg-error ".E0658." "" { target *-*-* } }

    // Opt expr
    let _x = [#[identity_attr] 2]; // { dg-error ".E0658." "" { target *-*-* } }

    // Expr macro
    let _x = #[identity_attr] println!();
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main() {}

