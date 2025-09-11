#![feature(unsafe_block_in_unsafe_fn)]
#![deny(unsafe_op_in_unsafe_fn)]
#![deny(unused_unsafe)]

unsafe fn unsf() {}
const PTR: *const () = std::ptr::null();
static mut VOID: () = ();

unsafe fn deny_level() {
    unsf();
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    *PTR;
// { dg-error "" "" { target *-*-* } .-1 }
    VOID = ();
// { dg-error "" "" { target *-*-* } .-1 }
}

// Check that `unsafe_op_in_unsafe_fn` works starting from the `warn` level.
#[warn(unsafe_op_in_unsafe_fn)]
#[deny(warnings)]
unsafe fn warning_level() {
    unsf();
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    *PTR;
// { dg-error "" "" { target *-*-* } .-1 }
    VOID = ();
// { dg-error "" "" { target *-*-* } .-1 }
}

unsafe fn explicit_block() {
    // no error
    unsafe {
        unsf();
        *PTR;
        VOID = ();
    }
}

unsafe fn two_explicit_blocks() {
    unsafe { unsafe { unsf() } }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[allow(unsafe_op_in_unsafe_fn)]
unsafe fn allow_level() {
    // lint allowed -> no error
    unsf();
    *PTR;
    VOID = ();

    unsafe { unsf() }
// { dg-error "" "" { target *-*-* } .-1 }
}

unsafe fn nested_allow_level() {
    #[allow(unsafe_op_in_unsafe_fn)]
    {
        // lint allowed -> no error
        unsf();
        *PTR;
        VOID = ();

        unsafe { unsf() }
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

fn main() {
    unsf();
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    #[allow(unsafe_op_in_unsafe_fn)]
    {
        unsf();
// { dg-error ".E0133." "" { target *-*-* } .-1 }
    }
}

