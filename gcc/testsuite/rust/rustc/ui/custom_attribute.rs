#![feature(stmt_expr_attributes)]

#[foo] // { dg-error "" "" { target *-*-* } }
fn main() {
    #[foo] // { dg-error "" "" { target *-*-* } }
    let x = ();
    #[foo] // { dg-error "" "" { target *-*-* } }
    x
}

