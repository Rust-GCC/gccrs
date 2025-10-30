// run-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct R;

impl R {
    fn method<const N: u8>(&self) -> u8 { N }
}
fn main() {
    assert_eq!(R.method::<1u8>(), 1);
}

