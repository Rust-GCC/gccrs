#![warn(unused)]
#![deny(unused_variables)]
#![deny(unused_assignments)]
#![allow(dead_code, non_camel_case_types, trivial_numeric_casts)]

use std::ops::AddAssign;

fn f1(x: isize) {
// { dg-error "" "" { target *-*-* } .-1 }
}

fn f1b(x: &mut isize) {
// { dg-error "" "" { target *-*-* } .-1 }
}

#[allow(unused_variables)]
fn f1c(x: isize) {}

fn f1d() {
    let x: isize;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn f2() {
    let x = 3;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn f3() {
    let mut x = 3;
// { dg-error "" "" { target *-*-* } .-1 }
    x += 4;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn f3b() {
    let mut z = 3;
// { dg-error "" "" { target *-*-* } .-1 }
    loop {
        z += 4;
    }
}

#[allow(unused_variables)]
fn f3c() {
    let mut z = 3;
    loop { z += 4; }
}

#[allow(unused_variables)]
#[allow(unused_assignments)]
fn f3d() {
    let mut x = 3;
    x += 4;
}

fn f4() {
    match Some(3) {
      Some(i) => {
// { dg-error "" "" { target *-*-* } .-1 }
      }
      None => {}
    }
}

enum tri {
    a(isize), b(isize), c(isize)
}

fn f4b() -> isize {
    match tri::a(3) {
      tri::a(i) | tri::b(i) | tri::c(i) => {
        i
      }
    }
}

fn f5a() {
    for x in 1..10 { }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn f5b() {
    for (x, _) in [1, 2, 3].iter().enumerate() { }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn f5c() {
    for (_, x) in [1, 2, 3].iter().enumerate() {
// { dg-error "" "" { target *-*-* } .-1 }
        continue;
        drop(*x as i32); // { dg-warning "" "" { target *-*-* } }
    }
}

struct View<'a>(&'a mut [i32]);

impl<'a> AddAssign<i32> for View<'a> {
    fn add_assign(&mut self, rhs: i32) {
        for lhs in self.0.iter_mut() {
            *lhs += rhs;
        }
    }
}

fn f6() {
    let mut array = [1, 2, 3];
    let mut v = View(&mut array);

    // ensure an error shows up for x even if lhs of an overloaded add assign

    let x;
// { dg-error "" "" { target *-*-* } .-1 }

    *({
        x = 0;  // { dg-error "" "" { target *-*-* } }
        &mut v
    }) += 1;
}


struct MutRef<'a>(&'a mut i32);

impl<'a> AddAssign<i32> for MutRef<'a> {
    fn add_assign(&mut self, rhs: i32) {
        *self.0 += rhs;
    }
}

fn f7() {
    let mut a = 1;
    {
        // `b` does not trigger unused_variables
        let mut b = MutRef(&mut a);
        b += 1;
    }
    drop(a);
}

fn main() {
}

