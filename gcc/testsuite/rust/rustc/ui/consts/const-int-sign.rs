fn main() {
    let x: &'static bool = &(5_i32.is_negative());
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    let y: &'static bool = &(5_i32.is_positive());
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

