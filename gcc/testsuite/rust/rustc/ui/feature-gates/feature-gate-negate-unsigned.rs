// Test that negating unsigned integers doesn't compile

struct S;
impl std::ops::Neg for S {
    type Output = u32;
    fn neg(self) -> u32 { 0 }
}

fn main() {
    let _max: usize = -1;
// { dg-error ".E0600." "" { target *-*-* } .-1 }

    let x = 5u8;
    let _y = -x;
// { dg-error ".E0600." "" { target *-*-* } .-1 }

    -S; // should not trigger the gate; issue 26840
}

