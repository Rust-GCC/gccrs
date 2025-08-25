fn main() {
    let x: f32 = 1; // { dg-error ".E0308." "" { target *-*-* } }
    let y: f32 = 1f64; // { dg-error ".E0308." "" { target *-*-* } }
}

