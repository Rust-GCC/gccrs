// Checks that pointers must not be used as the type of const params.
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

const A: u32 = 3;

struct Const<const P: *const u32>; // { dg-error "" "" { target *-*-* } }

impl<const P: *const u32> Const<P> { // { dg-error "" "" { target *-*-* } }
    fn get() -> u32 {
        unsafe {
            *P
        }
    }
}

fn main() {
    assert_eq!(Const::<{&A as *const _}>::get(), 3)
}

