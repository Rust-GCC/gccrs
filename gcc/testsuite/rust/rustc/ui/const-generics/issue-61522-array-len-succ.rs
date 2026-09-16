// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

pub struct MyArray<const COUNT: usize>([u8; COUNT + 1]);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

impl<const COUNT: usize> MyArray<COUNT> {
    fn inner(&self) -> &[u8; COUNT + 1] {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        &self.0
    }
}

fn main() {}

