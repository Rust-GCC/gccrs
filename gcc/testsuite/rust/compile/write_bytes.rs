// { dg-do compile }
// { dg-options "-fdump-tree-gimple" }

#![feature(no_core, intrinsics, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn write_bytes<T>(dst: *mut T, val: u8, count: usize);
}

// Test 1: 1-byte type. Should lower to __builtin_memset(ptr, 0, 5)
pub unsafe fn test_memset_u8(ptr: *mut u8) {
    write_bytes(ptr, 0, 5);
}

// Test 2: 4-byte type. Should lower to __builtin_memset(ptr, 0, 20)
pub unsafe fn test_memset_u32(ptr: *mut u32) {
    write_bytes(ptr, 0, 5);
}

// Assert that the backend successfully generated the built-in exactly twice
// { dg-final { scan-tree-dump-times "__builtin_memset" 2 "gimple" } }