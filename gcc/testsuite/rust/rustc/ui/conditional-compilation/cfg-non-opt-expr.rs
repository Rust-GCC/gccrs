#![feature(stmt_expr_attributes)]
#![feature(custom_test_frameworks)]

fn main() {
    let _ = #[cfg(unset)] ();
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = 1 + 2 + #[cfg(unset)] 3;
// { dg-error "" "" { target *-*-* } .-1 }
    let _ = [1, 2, 3][#[cfg(unset)] 1];
// { dg-error "" "" { target *-*-* } .-1 }
}

