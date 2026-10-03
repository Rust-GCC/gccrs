#[repr(packed)]
pub struct Good {
    data: &'static u32,
    data2: [&'static u32; 2],
    aligned: [u8; 32],
}

#[deny(safe_packed_borrows)]
fn main() {
    let good = Good {
        data: &0,
        data2: [&0, &0],
        aligned: [0; 32]
    };

    unsafe {
        let _ = &good.data; // ok
        let _ = &good.data2[0]; // ok
    }

    let _ = &good.data; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    let _ = &good.data2[0]; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    let _ = &*good.data; // ok, behind a pointer
    let _ = &good.aligned; // ok, has align 1
    let _ = &good.aligned[2]; // ok, has align 1
}

