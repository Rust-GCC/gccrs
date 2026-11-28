// Check that Polonius borrow check works for simple cases.
// ignore-compare-mode-nll
// compile-flags: -Z borrowck=mir -Zpolonius

pub fn return_ref_to_local() -> &'static i32 {
    let x = 0;
    &x // { dg-error ".E0515." "" { target *-*-* } }
}

pub fn use_while_mut() {
    let mut x = 0;
    let y = &mut x;
    let z = x; // { dg-error ".E0503." "" { target *-*-* } }
    let w = y;
}

pub fn use_while_mut_fr(x: &mut i32) -> &mut i32 {
    let y = &mut *x;
    let z = x; // { dg-error ".E0505." "" { target *-*-* } }
    y
}

// Cases like this are why we have Polonius.
pub fn position_dependent_outlives(x: &mut i32, cond: bool) -> &mut i32 {
    let y = &mut *x;
    if cond {
        return y;
    } else {
        *x = 0;
        return x;
    }
}

fn foo<'a, 'b>(p: &'b &'a mut usize) -> &'b usize {
    p
}

// Check that we create constraints for well-formedness of function arguments
fn well_formed_function_inputs() {
    let s = &mut 1;
    let r = &mut *s;
    let tmp = foo(&r);
    s; // { dg-error ".E0505." "" { target *-*-* } }
    tmp;
}

fn main() {}

