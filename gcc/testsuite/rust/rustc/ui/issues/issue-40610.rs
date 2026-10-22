fn f(_: &[f32]) {}

fn main() {
    () + f(&[1.0]);
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

