#![feature(extern_types)]
#![feature(core_intrinsics)]
#![feature(const_size_of_val, const_align_of_val)]

use std::intrinsics::{size_of_val, min_align_of_val};

extern {
    type Opaque;
}

const _SIZE: usize = unsafe { size_of_val(&4 as *const i32 as *const Opaque) }; // { dg-error "" "" { target *-*-* } }
const _ALIGN: usize = unsafe { min_align_of_val(&4 as *const i32 as *const Opaque) }; // { dg-error "" "" { target *-*-* } }

fn main() {}

