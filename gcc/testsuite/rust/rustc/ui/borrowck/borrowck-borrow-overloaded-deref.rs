// Test how overloaded deref interacts with borrows when only
// Deref and not DerefMut is implemented.

use std::ops::Deref;
use std::rc::Rc;

fn deref_imm(x: Rc<isize>) {
    let __isize = &*x;
}

fn deref_mut1(x: Rc<isize>) {
    let __isize = &mut *x; // { dg-error ".E0596." "" { target *-*-* } }
}

fn deref_mut2(mut x: Rc<isize>) {
    let __isize = &mut *x; // { dg-error ".E0596." "" { target *-*-* } }
}

fn deref_extend<'a>(x: &'a Rc<isize>) -> &'a isize {
    &**x
}

fn deref_extend_mut1<'a>(x: &'a Rc<isize>) -> &'a mut isize {
    &mut **x // { dg-error ".E0596." "" { target *-*-* } }
}

fn deref_extend_mut2<'a>(x: &'a mut Rc<isize>) -> &'a mut isize {
    &mut **x // { dg-error ".E0596." "" { target *-*-* } }
}

fn assign1<'a>(x: Rc<isize>) {
    *x = 3; // { dg-error ".E0594." "" { target *-*-* } }
}

fn assign2<'a>(x: &'a Rc<isize>) {
    **x = 3; // { dg-error ".E0594." "" { target *-*-* } }
}

fn assign3<'a>(x: &'a mut Rc<isize>) {
    **x = 3; // { dg-error ".E0594." "" { target *-*-* } }
}

pub fn main() {}

