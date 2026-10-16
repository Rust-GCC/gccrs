#![feature(fn_traits)]

struct Test;

impl FnOnce<(u32, u32)> for Test {
// { dg-error ".E0183." "" { target *-*-* } .-1 }
// { dg-error ".E0183." "" { target *-*-* } .-2 }
    type Output = u32;

    extern "rust-call" fn call_once(self, (a, b): (u32, u32)) -> u32 {
        a + b
    }
// { dg-error ".E0658." "" { target *-*-* } .-3 }
}

fn main() {
    assert_eq!(Test(1u32, 2u32), 3u32);
}

