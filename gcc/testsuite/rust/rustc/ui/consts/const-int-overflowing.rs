fn main() {
    let x: &'static (i32, bool) = &(5_i32.overflowing_add(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let y: &'static (i32, bool) = &(5_i32.overflowing_sub(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let z: &'static (i32, bool) = &(5_i32.overflowing_mul(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

