const fn bad_const_fn_deref_raw(x: *mut usize) -> &'static usize { unsafe { &*x } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const unsafe fn bad_const_unsafe_deref_raw(x: *mut usize) -> usize { *x }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const unsafe fn bad_const_unsafe_deref_raw_ref(x: *mut usize) -> &'static usize { &*x }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

const unsafe fn no_union() {
    union Foo { x: (), y: () }
    Foo { x: () }.y
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

