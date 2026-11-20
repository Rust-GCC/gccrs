struct RGB { r: f64, g: f64, b: f64 }

fn main() {
    let (r, g, c): (f32, f32, f32) = (0., 0., 0.);
    let _ = RGB { r, g, c };
// { dg-error ".E0560." "" { target *-*-* } .-1 }
// { dg-error ".E0560." "" { target *-*-* } .-2 }
// { dg-error ".E0560." "" { target *-*-* } .-3 }
}

