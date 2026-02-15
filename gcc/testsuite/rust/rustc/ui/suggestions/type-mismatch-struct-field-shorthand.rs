// run-rustfix
#![allow(dead_code)]

struct RGB { r: f64, g: f64, b: f64 }

fn main() {
    let (r, g, b): (f32, f32, f32) = (0., 0., 0.);
    let _ = RGB { r, g, b };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
}

