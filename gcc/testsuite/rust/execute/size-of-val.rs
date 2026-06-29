#![feature(no_core, intrinsics, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn size_of_val<T: ?Sized>(_: *const T) -> usize;
}

trait TraitA {
    fn foo(&self) {}
}

struct StructA {
    _a: i32,
    _b: i32,
}

impl TraitA for StructA {}
impl TraitA for u8 {}

fn main() -> i32 {
    let val_i32: i32 = 32;
    let size_i32 = unsafe { size_of_val(&val_i32 as *const i32) };

    let val_struct = StructA { _a: 1, _b: 2 };
    let size_struct = unsafe { size_of_val(&val_struct as *const StructA) };

    let arr: [i32; 3] = [10, 20, 30];
    let val_slice: &[i32] = &arr;
    let size_slice = unsafe { size_of_val(val_slice as *const [i32]) };

    let val_str: &str = "gccrs";
    let size_str = unsafe { size_of_val(val_str as *const str) };

    let val_dyn_struct = &val_struct as &dyn TraitA;
    let size_dyn_struct = unsafe { size_of_val(val_dyn_struct as *const dyn TraitA) };

    let val_u8: u8 = 7;
    let size_u8 = unsafe { size_of_val(&val_u8 as *const u8) };

    let val_dyn_u8 = &val_u8 as &dyn TraitA;
    let size_dyn_u8 = unsafe { size_of_val(val_dyn_u8 as *const dyn TraitA) };

    if size_i32 != 4 {
        1
    } else if size_struct != 8 {
        2
    } else if size_slice != 12 {
        3
    } else if size_str != 5 {
        4
    } else if size_dyn_struct != 8 {
        5
    } else if size_u8 != 1 {
        6
    } else if size_dyn_u8 != 1 {
        7
    } else {
        0
    }
}
