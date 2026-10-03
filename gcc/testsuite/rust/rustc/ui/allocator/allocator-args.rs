use std::alloc::{GlobalAlloc, Layout};

struct A;

unsafe impl GlobalAlloc for A {
    unsafe fn alloc(&self, _: Layout) -> *mut u8 { panic!() }
    unsafe fn dealloc(&self, _: *mut u8, _: Layout) { panic!() }
}

#[global_allocator(malloc)] // { dg-error "" "" { target *-*-* } }
static S: A = A;

fn main() {}

