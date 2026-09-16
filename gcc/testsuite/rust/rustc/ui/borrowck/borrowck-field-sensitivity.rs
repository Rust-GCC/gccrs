#![feature(box_syntax)]

struct A { a: isize, b: Box<isize> }

fn deref_after_move() {
    let x = A { a: 1, b: box 2 };
    drop(x.b);
    drop(*x.b); // { dg-error ".E0382." "" { target *-*-* } }
}

fn deref_after_fu_move() {
    let x = A { a: 1, b: box 2 };
    let y = A { a: 3, .. x };
    drop(*x.b); // { dg-error ".E0382." "" { target *-*-* } }
}

fn borrow_after_move() {
    let x = A { a: 1, b: box 2 };
    drop(x.b);
    let p = &x.b; // { dg-error ".E0382." "" { target *-*-* } }
    drop(**p);
}

fn borrow_after_fu_move() {
    let x = A { a: 1, b: box 2 };
    let _y = A { a: 3, .. x };
    let p = &x.b; // { dg-error ".E0382." "" { target *-*-* } }
    drop(**p);
}

fn move_after_borrow() {
    let x = A { a: 1, b: box 2 };
    let p = &x.b;
    drop(x.b); // { dg-error ".E0505." "" { target *-*-* } }
    drop(**p);
}

fn fu_move_after_borrow() {
    let x = A { a: 1, b: box 2 };
    let p = &x.b;
    let _y = A { a: 3, .. x }; // { dg-error ".E0505." "" { target *-*-* } }
    drop(**p);
}

fn mut_borrow_after_mut_borrow() {
    let mut x = A { a: 1, b: box 2 };
    let p = &mut x.a;
    let q = &mut x.a; // { dg-error ".E0499." "" { target *-*-* } }
    drop(*p);
    drop(*q);
}

fn move_after_move() {
    let x = A { a: 1, b: box 2 };
    drop(x.b);
    drop(x.b);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_after_fu_move() {
    let x = A { a: 1, b: box 2 };
    let _y = A { a: 3, .. x };
    drop(x.b);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn fu_move_after_move() {
    let x = A { a: 1, b: box 2 };
    drop(x.b);
    let _z = A { a: 3, .. x };  // { dg-error ".E0382." "" { target *-*-* } }
}

fn fu_move_after_fu_move() {
    let x = A { a: 1, b: box 2 };
    let _y = A { a: 3, .. x };
    let _z = A { a: 4, .. x };  // { dg-error ".E0382." "" { target *-*-* } }
}

// The following functions aren't yet accepted, but they should be.

fn copy_after_field_assign_after_uninit() {
    let mut x: A;
    x.a = 1; // { dg-error ".E0381." "" { target *-*-* } }
    drop(x.a);
}

fn borrow_after_field_assign_after_uninit() {
    let mut x: A;
    x.a = 1; // { dg-error ".E0381." "" { target *-*-* } }
    let p = &x.a;
    drop(*p);
}

fn move_after_field_assign_after_uninit() {
    let mut x: A;
    x.b = box 1; // { dg-error ".E0381." "" { target *-*-* } }
    drop(x.b);
}

fn main() {
    deref_after_move();
    deref_after_fu_move();

    borrow_after_move();
    borrow_after_fu_move();
    move_after_borrow();
    fu_move_after_borrow();
    mut_borrow_after_mut_borrow();

    move_after_move();
    move_after_fu_move();
    fu_move_after_move();
    fu_move_after_fu_move();

    copy_after_field_assign_after_uninit();
    borrow_after_field_assign_after_uninit();
    move_after_field_assign_after_uninit();
}

