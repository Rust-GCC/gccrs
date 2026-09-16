fn main() { let x = 1.0_f32 || 2.0_f32; }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

