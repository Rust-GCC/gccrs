#![feature(stmt_expr_attributes)]

#![deny(unused_doc_comments)]

macro_rules! mac {
    () => {}
}

/// foo // { dg-error "" "" { target *-*-* } }
mac!();

fn foo() {
    /// a // { dg-error "" "" { target *-*-* } }
    let x = 12;

    /// multi-line // { dg-error "" "" { target *-*-* } }
    /// doc comment
    /// that is unused
    match x {
        /// c // { dg-error "" "" { target *-*-* } }
        1 => {},
        _ => {}
    }

    /// foo // { dg-error "" "" { target *-*-* } }
    unsafe {}

    #[doc = "foo"] // { dg-error "" "" { target *-*-* } }
    #[doc = "bar"] // { dg-error "" "" { target *-*-* } }
    3;

    /// bar // { dg-error "" "" { target *-*-* } }
    mac!();

    let x = /** comment */ 47; // { dg-error "" "" { target *-*-* } }

    /// dox // { dg-error "" "" { target *-*-* } }
    {

    }
}

fn main() {
    foo();
}

