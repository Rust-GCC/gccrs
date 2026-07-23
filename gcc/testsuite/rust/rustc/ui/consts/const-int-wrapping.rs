fn main() {
    let x: &'static i32 = &(5_i32.wrapping_add(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let y: &'static i32 = &(5_i32.wrapping_sub(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let z: &'static i32 = &(5_i32.wrapping_mul(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let a: &'static i32 = &(5_i32.wrapping_shl(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let b: &'static i32 = &(5_i32.wrapping_shr(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

