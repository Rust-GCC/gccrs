#![feature(untagged_unions)]
#![allow(unused)]

use std::ops::{Deref, DerefMut};

#[derive(Default)]
struct MockBox<T> {
    value: [T; 1],
}

impl<T> MockBox<T> {
    fn new(value: T) -> Self { MockBox { value: [value] } }
}

impl<T> Deref for MockBox<T> {
    type Target = T;
    fn deref(&self) -> &T { &self.value[0] }
}

impl<T> DerefMut for MockBox<T> {
    fn deref_mut(&mut self) -> &mut T { &mut self.value[0] }
}

#[derive(Default)]
struct MockVec<T> {
    value: [T; 0],
}

impl<T> MockVec<T> {
    fn new() -> Self { MockVec { value: [] } }
}

impl<T> Deref for MockVec<T> {
    type Target = [T];
    fn deref(&self) -> &[T] { &self.value }
}

impl<T> DerefMut for MockVec<T> {
    fn deref_mut(&mut self) -> &mut [T] { &mut self.value }
}


union U {
    x: ((MockVec<u8>, MockVec<u8>), MockVec<u8>),
    y: MockBox<MockVec<u8>>,
}

fn use_borrow<T>(_: &T) {}

unsafe fn parent_sibling_borrow() {
    let mut u = U { x: ((MockVec::new(), MockVec::new()), MockVec::new()) };
    let a = &mut u.x.0;
    let b = &u.y; // { dg-error ".E0502." "" { target *-*-* } }
    use_borrow(a);
}

unsafe fn parent_sibling_move() {
    let u = U { x: ((MockVec::new(), MockVec::new()), MockVec::new()) };
    let a = u.x.0;
    let b = u.y; // { dg-error ".E0382." "" { target *-*-* } }
}

unsafe fn grandparent_sibling_borrow() {
    let mut u = U { x: ((MockVec::new(), MockVec::new()), MockVec::new()) };
    let a = &mut (u.x.0).0;
    let b = &u.y; // { dg-error ".E0502." "" { target *-*-* } }
    use_borrow(a);
}

unsafe fn grandparent_sibling_move() {
    let u = U { x: ((MockVec::new(), MockVec::new()), MockVec::new()) };
    let a = (u.x.0).0;
    let b = u.y; // { dg-error ".E0382." "" { target *-*-* } }
}

unsafe fn deref_sibling_borrow() {
    let mut u = U { y: MockBox::default() };
    let a = &mut *u.y;
    let b = &u.x; // { dg-error ".E0502." "" { target *-*-* } }
    use_borrow(a);
}

unsafe fn deref_sibling_move() {
    let u = U { x: ((MockVec::new(), MockVec::new()), MockVec::new()) };
    // No way to test deref-move without Box in union
    // let a = *u.y;
    // let b = u.x; ERROR use of moved value: `u`
}


fn main() {}

