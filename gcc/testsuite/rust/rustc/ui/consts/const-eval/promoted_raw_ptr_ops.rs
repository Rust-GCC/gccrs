#![feature(const_raw_ptr_to_usize_cast, const_raw_ptr_deref)]

fn main() {
    let x: &'static bool = &(42 as *const i32 == 43 as *const i32);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let y: &'static usize = &(&1 as *const i32 as usize + 1);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let z: &'static i32 = &(unsafe { *(42 as *const i32) });
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let a: &'static bool = &(main as fn() == main as fn());
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

