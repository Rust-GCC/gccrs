fn main() {
    let x: &'static i32 = &(5_i32.rotate_left(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let y: &'static i32 = &(5_i32.rotate_right(3));
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

