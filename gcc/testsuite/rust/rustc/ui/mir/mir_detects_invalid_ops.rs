// build-fail

fn main() {
    divide_by_zero();
    mod_by_zero();
    oob_error_for_slices();
}

fn divide_by_zero() {
    let y = 0;
    let _z = 1 / y; // { dg-error "" "" { target *-*-* } }
}

fn mod_by_zero() {
    let y = 0;
    let _z = 1 % y; // { dg-error "" "" { target *-*-* } }
}

fn oob_error_for_slices() {
    let a: *const [_] = &[1, 2, 3];
    unsafe {
        let _b = (*a)[3];
    }
}

