// Check that bogus field access is non-fatal
fn main() {
    let x = 0;
    let _ = x.foo; // { dg-error ".E0610." "" { target *-*-* } }
    let _ = x.bar; // { dg-error ".E0610." "" { target *-*-* } }
}

