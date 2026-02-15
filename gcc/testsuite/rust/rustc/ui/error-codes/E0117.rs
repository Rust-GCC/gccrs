impl Drop for u32 {} // { dg-error ".E0117." "" { target *-*-* } }
// { dg-error ".E0120." "" { target *-*-* } .-2 }

fn main() {
}

