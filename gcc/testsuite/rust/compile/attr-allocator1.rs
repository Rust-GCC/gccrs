// { dg-additional-options "-fdump-tree-gimple" }
#![feature(no_core, rustc_attrs)]
#![no_core]

#[rustc_allocator]
#[rustc_allocator_nounwind]
fn _foo() -> *mut u8 {
    0 as *mut u8
}

// { dg-final { scan-tree-dump "malloc" "gimple" } }
// { dg-final { scan-tree-dump "nothrow" "gimple" } }
