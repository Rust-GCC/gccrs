// { dg-require-effective-target lp64 }
#![feature(no_core, intrinsics, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn min_align_of_val<T: ?Sized>(_: *const T) -> usize;
}

trait TraitA {
    fn foo(&self) {}
}

struct StructA {
    _a: i32,
    _b: i16,
}

impl TraitA for StructA {}
impl TraitA for u8 {}

fn main() -> i32 {
    let val_i32: i32 = 32;
    let align_i32 = unsafe { min_align_of_val(&val_i32 as *const i32) };

    let val_struct = StructA { _a: 1, _b: 2 };
    let align_struct = unsafe { min_align_of_val(&val_struct as *const StructA) };

    let arr: [i32; 3] = [10, 20, 30];
    let val_slice: &[i32] = &arr;
    let align_slice = unsafe { min_align_of_val(val_slice as *const [i32]) };

    let val_str: &str = "gccrs";
    let align_str = unsafe { min_align_of_val(val_str as *const str) };

    let val_dyn_struct = &val_struct as &dyn TraitA;
    let align_dyn_struct = unsafe { min_align_of_val(val_dyn_struct as *const dyn TraitA) };

    let val_u8: u8 = 7;
    let align_u8 = unsafe { min_align_of_val(&val_u8 as *const u8) };

    let val_dyn_u8 = &val_u8 as &dyn TraitA;
    let align_dyn_u8 = unsafe { min_align_of_val(val_dyn_u8 as *const dyn TraitA) };

    if align_i32 != 4 {
        1
    } else if align_struct != 4 {
        2
    } else if align_slice != 4 {
        3
    } else if align_str != 1 {
        4
    } else if align_dyn_struct != 4 {
        5
    } else if align_u8 != 1 {
        6
    } else if align_dyn_u8 != 1 {
        7
    } else {
        0
    }
}
