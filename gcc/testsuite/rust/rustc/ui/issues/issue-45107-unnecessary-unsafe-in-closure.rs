#[deny(unused_unsafe)]
fn main() {
    let mut v = Vec::<i32>::with_capacity(24);

    unsafe {
        let f = |v: &mut Vec<_>| {
            unsafe { // { dg-error "" "" { target *-*-* } }
                v.set_len(24);
                |w: &mut Vec<u32>| { unsafe { // { dg-error "" "" { target *-*-* } }
                    w.set_len(32);
                } };
            }
            |x: &mut Vec<u32>| { unsafe { // { dg-error "" "" { target *-*-* } }
                x.set_len(40);
            } };
        };

        v.set_len(0);
        f(&mut v);
    }

    |y: &mut Vec<u32>| { unsafe {
        y.set_len(48);
    } };
}

