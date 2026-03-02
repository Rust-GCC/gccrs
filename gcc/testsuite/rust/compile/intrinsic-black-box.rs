// { dg-do compile }
// { dg-options "-O2 -fdump-tree-gimple" }
#![feature(intrinsics, lang_items, no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn black_box<T>(dummy: T) -> T;
}

pub fn main() {
    let _ = unsafe { black_box(42) };

    // Scan the gimple dump to ensure the volatile inline assembly was generated
    // { dg-final { scan-tree-dump-times "__asm__ __volatile__" 1 "gimple" } }
}