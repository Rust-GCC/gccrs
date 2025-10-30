// This tests that we can't modify Box<&mut T> contents while they
// are borrowed (#14498).
//
// Also includes tests of the errors reported when the Box in question
// is immutable (#14270).

#![feature(box_syntax)]

struct A { a: isize }
struct B<'a> { a: Box<&'a mut isize> }

fn indirect_write_to_imm_box() {
    let mut x: isize = 1;
    let y: Box<_> = box &mut x;
    let p = &y;
    ***p = 2; // { dg-error ".E0594." "" { target *-*-* } }
    drop(p);
}

fn borrow_in_var_from_var() {
    let mut x: isize = 1;
    let mut y: Box<_> = box &mut x;
    let p = &y;
    let q = &***p;
    **y = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_var_from_var_via_imm_box() {
    let mut x: isize = 1;
    let y: Box<_> = box &mut x;
    let p = &y;
    let q = &***p;
    **y = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_var_from_field() {
    let mut x = A { a: 1 };
    let mut y: Box<_> = box &mut x.a;
    let p = &y;
    let q = &***p;
    **y = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_var_from_field_via_imm_box() {
    let mut x = A { a: 1 };
    let y: Box<_> = box &mut x.a;
    let p = &y;
    let q = &***p;
    **y = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_field_from_var() {
    let mut x: isize = 1;
    let mut y = B { a: box &mut x };
    let p = &y.a;
    let q = &***p;
    **y.a = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_field_from_var_via_imm_box() {
    let mut x: isize = 1;
    let y = B { a: box &mut x };
    let p = &y.a;
    let q = &***p;
    **y.a = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_field_from_field() {
    let mut x = A { a: 1 };
    let mut y = B { a: box &mut x.a };
    let p = &y.a;
    let q = &***p;
    **y.a = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn borrow_in_field_from_field_via_imm_box() {
    let mut x = A { a: 1 };
    let y = B { a: box &mut x.a };
    let p = &y.a;
    let q = &***p;
    **y.a = 2; // { dg-error ".E0506." "" { target *-*-* } }
    drop(p);
    drop(q);
}

fn main() {
    indirect_write_to_imm_box();
    borrow_in_var_from_var();
    borrow_in_var_from_var_via_imm_box();
    borrow_in_var_from_field();
    borrow_in_var_from_field_via_imm_box();
    borrow_in_field_from_var();
    borrow_in_field_from_var_via_imm_box();
    borrow_in_field_from_field();
    borrow_in_field_from_field_via_imm_box();
}

