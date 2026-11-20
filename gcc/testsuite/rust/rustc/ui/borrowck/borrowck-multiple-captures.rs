#![feature(box_syntax)]

use std::thread;

fn borrow<T>(_: &T) { }

fn different_vars_after_borrows() {
    let x1: Box<_> = box 1;
    let p1 = &x1;
    let x2: Box<_> = box 2;
    let p2 = &x2;
    thread::spawn(move|| {
// { dg-error ".E0505." "" { target *-*-* } .-1 }
// { dg-error ".E0505." "" { target *-*-* } .-2 }
        drop(x1);
        drop(x2);
    });
    borrow(&*p1);
    borrow(&*p2);
}

fn different_vars_after_moves() {
    let x1: Box<_> = box 1;
    drop(x1);
    let x2: Box<_> = box 2;
    drop(x2);
    thread::spawn(move|| {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
        drop(x1);
        drop(x2);
    });
}

fn same_var_after_borrow() {
    let x: Box<_> = box 1;
    let p = &x;
    thread::spawn(move|| {
// { dg-error ".E0505." "" { target *-*-* } .-1 }
        drop(x);
        drop(x); // { dg-error ".E0382." "" { target *-*-* } }
    });
    borrow(&*p);
}

fn same_var_after_move() {
    let x: Box<_> = box 1;
    drop(x);
    thread::spawn(move|| {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        drop(x);
        drop(x); // { dg-error ".E0382." "" { target *-*-* } }
    });
}

fn main() {
    different_vars_after_borrows();
    different_vars_after_moves();
    same_var_after_borrow();
    same_var_after_move();
}

