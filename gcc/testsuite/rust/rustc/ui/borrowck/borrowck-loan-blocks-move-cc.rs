#![feature(box_syntax)]

use std::thread;



fn borrow<F>(v: &isize, f: F) where F: FnOnce(&isize) {
    f(v);
}

fn box_imm() {
    let v: Box<_> = box 3;
    let w = &v;
    thread::spawn(move|| {
// { dg-error ".E0505." "" { target *-*-* } .-1 }
        println!("v={}", *v);
    });
    w.use_ref();
}

fn box_imm_explicit() {
    let v: Box<_> = box 3;
    let w = &v;
    thread::spawn(move|| {
// { dg-error ".E0505." "" { target *-*-* } .-1 }
        println!("v={}", *v);
    });
    w.use_ref();
}

fn main() {
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

