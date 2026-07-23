#![feature(const_raw_ptr_to_usize_cast)]

fn main() {
    [(); &(static |x| {}) as *const _ as usize];
// { dg-error ".E0282." "" { target *-*-* } .-1 }
// { dg-error ".E0282." "" { target *-*-* } .-2 }
    [(); &(static || {}) as *const _ as usize];
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

