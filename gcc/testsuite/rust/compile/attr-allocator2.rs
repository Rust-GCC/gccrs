#![feature(no_core, rustc_attrs)]
#![no_core]

#[rustc_allocator(invalid)] // { dg-error "malformed .rustc_allocator. attribute input" }
pub fn _foo() -> *mut u8 {
    0 as *mut u8
}

#[rustc_allocator_nounwind(invalid)] // { dg-error "malformed .rustc_allocator_nounwind. attribute input" }
pub fn _bar() {}
