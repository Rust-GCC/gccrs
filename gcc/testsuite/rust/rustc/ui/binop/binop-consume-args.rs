// Test that binary operators consume their arguments

use std::ops::{Add, Sub, Mul, Div, Rem, BitAnd, BitXor, BitOr, Shl, Shr};

fn add<A: Add<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs + rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn sub<A: Sub<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs - rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn mul<A: Mul<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs * rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn div<A: Div<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs / rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn rem<A: Rem<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs % rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn bitand<A: BitAnd<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs & rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn bitor<A: BitOr<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs | rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn bitxor<A: BitXor<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs ^ rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn shl<A: Shl<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs << rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn shr<A: Shr<B, Output=()>, B>(lhs: A, rhs: B) {
    lhs >> rhs;
    drop(lhs);  // { dg-error ".E0382." "" { target *-*-* } }
    drop(rhs);  // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {}

