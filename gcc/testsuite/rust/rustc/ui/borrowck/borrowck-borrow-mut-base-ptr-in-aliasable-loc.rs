// Test that attempt to reborrow an `&mut` pointer in an aliasable
// location yields an error.
//
// Example from compiler/rustc_borrowck/borrowck/README.md

fn foo(t0: & &mut isize) {
    let t1 = t0;
    let p: &isize = &**t0;
    **t1 = 22; // { dg-error ".E0594." "" { target *-*-* } }
}

fn foo3(t0: &mut &mut isize) {
    let t1 = &mut *t0;
    let p: &isize = &**t0; // { dg-error ".E0502." "" { target *-*-* } }
    **t1 = 22;
}

fn foo4(t0: & &mut isize) {
    let x:  &mut isize = &mut **t0; // { dg-error ".E0596." "" { target *-*-* } }
    *x += 1;
}

fn main() {
}

