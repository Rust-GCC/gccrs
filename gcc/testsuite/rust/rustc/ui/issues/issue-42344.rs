static TAB: [&mut [u8]; 0] = [];

pub unsafe fn test() {
    TAB[0].iter_mut();
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

pub fn main() {}

