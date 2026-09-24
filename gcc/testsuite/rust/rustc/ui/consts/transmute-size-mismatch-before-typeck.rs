#![feature(const_transmute)]

// normalize-stderr-64bit "64 bits" -> "word size"
// normalize-stderr-32bit "32 bits" -> "word size"
// normalize-stderr-64bit "128 bits" -> "2 * word size"
// normalize-stderr-32bit "64 bits" -> "2 * word size"

fn main() {
    match &b""[..] {
        ZST => {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    }
}

const ZST: &[u8] = unsafe { std::mem::transmute(1usize) };
// { dg-error ".E0512." "" { target *-*-* } .-1 }
// { dg-error ".E0512." "" { target *-*-* } .-2 }

// Once the `any use of this value will cause an error` disappears in this test, make sure to
// remove the `TransmuteSizeDiff` error variant and make its emitter site an assertion again.

