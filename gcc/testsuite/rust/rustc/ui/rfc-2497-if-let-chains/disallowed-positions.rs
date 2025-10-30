// Here we test that `lowering` behaves correctly wrt. `let $pats = $expr` expressions.
//
// We want to make sure that `let` is banned in situations other than:
//
// expr =
//   | ...
//   | "if" expr_with_let block {"else" block}?
//   | {label ":"}? while" expr_with_let block
//   ;
//
// expr_with_let =
//   | "let" top_pats "=" expr
//   | expr_with_let "&&" expr_with_let
//   | "(" expr_with_let ")"
//   | expr
//   ;
//
// To that end, we check some positions which is not part of the language above.

#![feature(const_generics)]
// { dg-warning "" "" { target *-*-* } .-1 }
#![feature(let_chains)] // Avoid inflating `.stderr` with overzealous gates in this test.
// { dg-warning "" "" { target *-*-* } .-1 }

#![allow(irrefutable_let_patterns)]

use std::ops::Range;

fn main() {}

fn nested_within_if_expr() {
    if &let 0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    if !let 0 = 0 {} // { dg-error "" "" { target *-*-* } }
    if *let 0 = 0 {} // { dg-error ".E0614." "" { target *-*-* } }
// { dg-error ".E0614." "" { target *-*-* } .-1 }
    if -let 0 = 0 {} // { dg-error ".E0600." "" { target *-*-* } }
// { dg-error ".E0600." "" { target *-*-* } .-1 }

    fn _check_try_binds_tighter() -> Result<(), ()> {
        if let 0 = 0? {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        Ok(())
    }
    if (let 0 = 0)? {} // { dg-error ".E0277." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    if true || let 0 = 0 {} // { dg-error "" "" { target *-*-* } }
    if (true || let 0 = 0) {} // { dg-error "" "" { target *-*-* } }
    if true && (true || let 0 = 0) {} // { dg-error "" "" { target *-*-* } }
    if true || (true && let 0 = 0) {} // { dg-error "" "" { target *-*-* } }

    let mut x = true;
    if x = let 0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    if true..(let 0 = 0) {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    if ..(let 0 = 0) {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    if (let 0 = 0).. {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    // Binds as `(let ... = true)..true &&/|| false`.
    if let Range { start: _, end: _ } = true..true && false {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
    if let Range { start: _, end: _ } = true..true || false {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }

    // Binds as `(let Range { start: F, end } = F)..(|| true)`.
    const F: fn() -> bool = || true;
    if let Range { start: F, end } = F..|| true {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    // Binds as `(let Range { start: true, end } = t)..(&&false)`.
    let t = &&true;
    if let Range { start: true, end } = t..&&false {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    if let true = let true = true {} // { dg-error "" "" { target *-*-* } }
}

fn nested_within_while_expr() {
    while &let 0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    while !let 0 = 0 {} // { dg-error "" "" { target *-*-* } }
    while *let 0 = 0 {} // { dg-error ".E0614." "" { target *-*-* } }
// { dg-error ".E0614." "" { target *-*-* } .-1 }
    while -let 0 = 0 {} // { dg-error ".E0600." "" { target *-*-* } }
// { dg-error ".E0600." "" { target *-*-* } .-1 }

    fn _check_try_binds_tighter() -> Result<(), ()> {
        while let 0 = 0? {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        Ok(())
    }
    while (let 0 = 0)? {} // { dg-error ".E0277." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    while true || let 0 = 0 {} // { dg-error "" "" { target *-*-* } }
    while (true || let 0 = 0) {} // { dg-error "" "" { target *-*-* } }
    while true && (true || let 0 = 0) {} // { dg-error "" "" { target *-*-* } }
    while true || (true && let 0 = 0) {} // { dg-error "" "" { target *-*-* } }

    let mut x = true;
    while x = let 0 = 0 {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    while true..(let 0 = 0) {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    while ..(let 0 = 0) {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    while (let 0 = 0).. {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    // Binds as `(let ... = true)..true &&/|| false`.
    while let Range { start: _, end: _ } = true..true && false {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
    while let Range { start: _, end: _ } = true..true || false {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }

    // Binds as `(let Range { start: F, end } = F)..(|| true)`.
    const F: fn() -> bool = || true;
    while let Range { start: F, end } = F..|| true {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    // Binds as `(let Range { start: true, end } = t)..(&&false)`.
    let t = &&true;
    while let Range { start: true, end } = t..&&false {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    while let true = let true = true {} // { dg-error "" "" { target *-*-* } }
}

fn not_error_because_clarified_intent() {
    if let Range { start: _, end: _ } = (true..true || false) { }

    if let Range { start: _, end: _ } = (true..true && false) { }

    while let Range { start: _, end: _ } = (true..true || false) { }

    while let Range { start: _, end: _ } = (true..true && false) { }
}

fn outside_if_and_while_expr() {
    &let 0 = 0; // { dg-error "" "" { target *-*-* } }

    !let 0 = 0; // { dg-error "" "" { target *-*-* } }
    *let 0 = 0; // { dg-error ".E0614." "" { target *-*-* } }
// { dg-error ".E0614." "" { target *-*-* } .-1 }
    -let 0 = 0; // { dg-error ".E0600." "" { target *-*-* } }
// { dg-error ".E0600." "" { target *-*-* } .-1 }

    fn _check_try_binds_tighter() -> Result<(), ()> {
        let 0 = 0?;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        Ok(())
    }
    (let 0 = 0)?; // { dg-error ".E0277." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    true || let 0 = 0; // { dg-error "" "" { target *-*-* } }
    (true || let 0 = 0); // { dg-error "" "" { target *-*-* } }
    true && (true || let 0 = 0); // { dg-error "" "" { target *-*-* } }

    let mut x = true;
    x = let 0 = 0; // { dg-error "" "" { target *-*-* } }

    true..(let 0 = 0); // { dg-error "" "" { target *-*-* } }
    ..(let 0 = 0); // { dg-error "" "" { target *-*-* } }
    (let 0 = 0)..; // { dg-error "" "" { target *-*-* } }

    (let Range { start: _, end: _ } = true..true || false);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    (let true = let true = true);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // Check function tail position.
    &let 0 = 0
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

// Let's make sure that `let` inside const generic arguments are considered.
fn inside_const_generic_arguments() {
    struct A<const B: bool>;
    impl<const B: bool> A<{B}> { const O: u32 = 5; }

    if let A::<{
        true && let 1 = 1 // { dg-error "" "" { target *-*-* } }
    }>::O = 5 {}

    while let A::<{
        true && let 1 = 1 // { dg-error "" "" { target *-*-* } }
    }>::O = 5 {}

    if A::<{
        true && let 1 = 1 // { dg-error "" "" { target *-*-* } }
    }>::O == 5 {}

    // In the cases above we have `ExprKind::Block` to help us out.
    // Below however, we would not have a block and so an implementation might go
    // from visiting expressions to types without banning `let` expressions down the tree.
    // This tests ensures that we are not caught by surprise should the parser
    // admit non-IDENT expressions in const generic arguments.

    if A::<
        true && let 1 = 1
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    >::O == 5 {}
}

