// Test that multi-byte unicode characters with missing parameters do not ICE.

fn main() {
  println!("\r¡{}")
// { dg-error "" "" { target *-*-* } .-1 }
}

