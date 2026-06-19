// A quick test of 'unsafe const fn' functionality

const unsafe fn dummy(v: u32) -> u32 {
    !v
}

const VAL: u32 = dummy(0xFFFF);
// { dg-error ".E0133." "" { target *-*-* } .-1 }

fn main() {
    assert_eq!(VAL, 0xFFFF0000);
}

