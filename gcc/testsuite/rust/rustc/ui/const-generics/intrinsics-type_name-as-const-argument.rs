// revisions: full min

#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]

#![feature(core_intrinsics)]
#![feature(const_type_name)]

trait Trait<const S: &'static str> {}
// { dg-error "" "" { target *-*-* } .-1 }

struct Bug<T>
where
    T: Trait<{std::intrinsics::type_name::<T>()}>
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
{
    t: T
}

fn main() {}

