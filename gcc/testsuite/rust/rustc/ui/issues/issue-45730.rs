use std::fmt;
fn main() {
    let x: *const _ = 0 as _; // { dg-error ".E0641." "" { target *-*-* } }

    let x: *const _ = 0 as *const _; // { dg-error ".E0641." "" { target *-*-* } }
    let y: Option<*const dyn fmt::Debug> = Some(x) as _;

    let x = 0 as *const i32 as *const _ as *mut _; // { dg-error ".E0641." "" { target *-*-* } }
}

