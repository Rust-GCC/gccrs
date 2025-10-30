#![feature(box_syntax)]

#[derive(Copy, Clone)]
struct A { a: isize, b: isize }

struct B { a: isize, b: Box<isize> }

fn var_copy_after_var_borrow() {
    let mut x: isize = 1;
    let p = &mut x;
    drop(x); // { dg-error ".E0503." "" { target *-*-* } }
    *p = 2;
}

fn var_copy_after_field_borrow() {
    let mut x = A { a: 1, b: 2 };
    let p = &mut x.a;
    drop(x); // { dg-error ".E0503." "" { target *-*-* } }
    *p = 3;
}

fn field_copy_after_var_borrow() {
    let mut x = A { a: 1, b: 2 };
    let p = &mut x;
    drop(x.a); // { dg-error ".E0503." "" { target *-*-* } }
    p.a = 3;
}

fn field_copy_after_field_borrow() {
    let mut x = A { a: 1, b: 2 };
    let p = &mut x.a;
    drop(x.a); // { dg-error ".E0503." "" { target *-*-* } }
    *p = 3;
}

fn fu_field_copy_after_var_borrow() {
    let mut x = A { a: 1, b: 2 };
    let p = &mut x;
    let y = A { b: 3, .. x }; // { dg-error ".E0503." "" { target *-*-* } }
    drop(y);
    p.a = 4;
}

fn fu_field_copy_after_field_borrow() {
    let mut x = A { a: 1, b: 2 };
    let p = &mut x.a;
    let y = A { b: 3, .. x }; // { dg-error ".E0503." "" { target *-*-* } }
    drop(y);
    *p = 4;
}

fn var_deref_after_var_borrow() {
    let mut x: Box<isize> = box 1;
    let p = &mut x;
    drop(*x); // { dg-error ".E0503." "" { target *-*-* } }
    **p = 2;
}

fn field_deref_after_var_borrow() {
    let mut x = B { a: 1, b: box 2 };
    let p = &mut x;
    drop(*x.b); // { dg-error ".E0503." "" { target *-*-* } }
    p.a = 3;
}

fn field_deref_after_field_borrow() {
    let mut x = B { a: 1, b: box 2 };
    let p = &mut x.b;
    drop(*x.b); // { dg-error ".E0503." "" { target *-*-* } }
    **p = 3;
}

fn main() {
    var_copy_after_var_borrow();
    var_copy_after_field_borrow();

    field_copy_after_var_borrow();
    field_copy_after_field_borrow();

    fu_field_copy_after_var_borrow();
    fu_field_copy_after_field_borrow();

    var_deref_after_var_borrow();
    field_deref_after_var_borrow();
    field_deref_after_field_borrow();
}

