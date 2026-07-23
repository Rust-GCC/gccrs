// revisions: full min

#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]

pub const fn is_zst<T: ?Sized>() -> usize {
    if std::mem::size_of::<T>() == 0 {
// { dg-error "" "" { target *-*-* } .-1 }
        1
    } else {
        0
    }
}

pub struct AtLeastByte<T: ?Sized> {
    value: T,
// { dg-error "" "" { target *-*-* } .-1 }
    pad: [u8; is_zst::<T>()],
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

