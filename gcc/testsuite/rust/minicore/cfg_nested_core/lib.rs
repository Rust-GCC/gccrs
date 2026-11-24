#![feature(no_core)]
#![no_core]

#[cfg(any(target_pointer_width = "64", target_pointer_width = "32"))]
pub type ifoo = i64;
#[cfg(any(target_pointer_width = "64", target_pointer_width = "32"))]
pub type ufoo = u64;
