// Regression test for #63952, shouldn't hang.

use std::usize;

#[repr(C)]
#[derive(Copy, Clone)]
struct SliceRepr {
    ptr: *const u8,
    len: usize,
}

union SliceTransmute {
    repr: SliceRepr,
    slice: &'static [u8],
}

// bad slice: length too big to even exist anywhere
const SLICE_WAY_TOO_LONG: &[u8] = unsafe { // { dg-error ".E0080." "" { target *-*-* } }
    SliceTransmute {
        repr: SliceRepr {
            ptr: &42,
            len: usize::MAX,
        },
    }
    .slice
};

fn main() {}

