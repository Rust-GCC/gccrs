#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

pub struct Struct<const N: usize>(pub [u8; N]);

pub type Alias = Struct<2>;

pub fn function(value: Struct<3>) -> u8 {
    value.0[0]
}

