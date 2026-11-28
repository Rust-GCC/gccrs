// We used to ICE when moving out of a `*mut T` or `*const T`.

struct T(u8);

static mut GLOBAL_MUT_T: T = T(0);

static GLOBAL_T: T = T(0);

fn imm_ref() -> &'static T {
    unsafe { &GLOBAL_T }
}

fn mut_ref() -> &'static mut T {
    unsafe { &mut GLOBAL_MUT_T }
}

fn mut_ptr() -> *mut T {
    unsafe { core::ptr::null_mut() }
}

fn const_ptr() -> *const T {
    unsafe { core::ptr::null() }
}

pub fn main() {
    let a = unsafe { *mut_ref() };
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    let b = unsafe { *imm_ref() };
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    let c = unsafe { *mut_ptr() };
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    let d = unsafe { *const_ptr() };
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

