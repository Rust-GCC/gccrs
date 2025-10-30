#![feature(core_intrinsics)]

fn main() {
    let (x, y) = (1u32, 2u32);
    let add = std::intrinsics::unchecked_add(x, y); // { dg-error ".E0133." "" { target *-*-* } }
    let sub = std::intrinsics::unchecked_sub(x, y); // { dg-error ".E0133." "" { target *-*-* } }
    let mul = std::intrinsics::unchecked_mul(x, y); // { dg-error ".E0133." "" { target *-*-* } }
}

