// Test that we check that user type annotations are well-formed, even in dead
// code.

#![feature(nll)]

fn uninit<'a>() {
    return;
    let x: &'static &'a ();                         // { dg-error "" "" { target *-*-* } }
}

fn var_type<'a>() {
    return;
    let x: &'static &'a () = &&();                  // { dg-error "" "" { target *-*-* } }
}

fn uninit_infer<'a>() {
    let x: &'static &'a _;                          // { dg-error "" "" { target *-*-* } }
    x = && ();
}

fn infer<'a>() {
    return;
    let x: &'static &'a _ = &&();                   // { dg-error "" "" { target *-*-* } }
}

fn uninit_no_var<'a>() {
    return;
    let _: &'static &'a ();                         // { dg-error "" "" { target *-*-* } }
}

fn no_var<'a>() {
    return;
    let _: &'static &'a () = &&();                  // { dg-error "" "" { target *-*-* } }
}

fn infer_no_var<'a>() {
    return;
    let _: &'static &'a _ = &&();                   // { dg-error "" "" { target *-*-* } }
}

trait X<'a, 'b> {}

struct C<'a, 'b, T: X<'a, 'b>>(T, &'a (), &'b ());

impl X<'_, '_> for i32 {}
impl<'a> X<'a, 'a> for () {}

// This type annotation is not well-formed because we substitute `()` for `_`.
fn required_substs<'a>() {
    return;
    let _: C<'static, 'a, _> = C((), &(), &());     // { dg-error "" "" { target *-*-* } }
}

fn main() {}

