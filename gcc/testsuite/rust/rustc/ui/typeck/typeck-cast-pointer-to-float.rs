fn main() {
    let x : i16 = 22;
    ((&x) as *const i16) as f32;
// { dg-error ".E0606." "" { target *-*-* } .-1 }
}

