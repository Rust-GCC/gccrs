// Basic test for liveness constraints: the region (`R1`) that appears
// in the type of `p` must include everything until `p` is dropped
// because of destructor. (Note that the stderr also identifies this
// destructor in the error message.)

// compile-flags:-Zborrowck=mir

#![allow(warnings)]
#![feature(dropck_eyepatch)]

fn use_x(_: usize) -> bool { true }

fn main() {
    let mut v = [1, 2, 3];
    let p: WrapMayNotDangle<&usize> = WrapMayNotDangle { value: &v[0] };
    if true {
        use_x(*p.value);
    } else {
        use_x(22);
        v[0] += 1; // { dg-error ".E0506." "" { target *-*-* } }
    }

    v[0] += 1; // { dg-error ".E0506." "" { target *-*-* } }
}

struct WrapMayNotDangle<T> {
    value: T
}

impl<T> Drop for WrapMayNotDangle<T> {
    fn drop(&mut self) { }
}

