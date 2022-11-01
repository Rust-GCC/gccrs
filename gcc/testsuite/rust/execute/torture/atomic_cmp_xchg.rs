trait Copy {}

extern "rust-intrinsic" {
    pub fn atomic_cxchg_seqcst_seqcst<T: Copy>(dst: *mut T, old: T, src: T) -> (T, bool);
}

fn perform_call(old: u32) -> (u32, bool) {
    let mut dst = 15u32;
    let new = 14u32;

    // FIXME: This is a little weird. We get a verification error during gimple checking about
    // non-trivial conversion between `struct (T=i32, bool)` and `struct (i32, bool)`
    // Destructuring and re-tupling fixes it for now. This probably happens because we create
    // the tuple type by hand instead of creating the proper `HIR::TupleType()` and compiling it
    let res = unsafe { atomic_cxchg_seqcst_seqcst(&mut dst, old, new) };

    (res.0, res.1)
}

fn main() -> u32 {
    let res0 = perform_call(15); // valid - should return {15, true}
    let res1 = perform_call(189); // invalid - should return {15, false}

    if res0.1 && !res1.1 {
        (res0.0 - 15) + (res1.0 - 15)
    } else {
        1 // return an error
    }
}
