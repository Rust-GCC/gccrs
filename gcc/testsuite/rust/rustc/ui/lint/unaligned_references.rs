#![deny(unaligned_references)]

#[repr(packed)]
pub struct Good {
    data: u64,
    ptr: &'static u64,
    data2: [u64; 2],
    aligned: [u8; 32],
}

fn main() {
    unsafe {
        let good = Good { data: 0, ptr: &0, data2: [0, 0], aligned: [0; 32] };

        let _ = &good.ptr; // { dg-error "" "" { target *-*-* } }
        let _ = &good.data; // { dg-error "" "" { target *-*-* } }
        // Error even when turned into raw pointer immediately.
        let _ = &good.data as *const _; // { dg-error "" "" { target *-*-* } }
        let _: *const _ = &good.data; // { dg-error "" "" { target *-*-* } }
        // Error on method call.
        let _ = good.data.clone(); // { dg-error "" "" { target *-*-* } }
        // Error for nested fields.
        let _ = &good.data2[0]; // { dg-error "" "" { target *-*-* } }

        let _ = &*good.ptr; // ok, behind a pointer
        let _ = &good.aligned; // ok, has align 1
        let _ = &good.aligned[2]; // ok, has align 1
    }
}

