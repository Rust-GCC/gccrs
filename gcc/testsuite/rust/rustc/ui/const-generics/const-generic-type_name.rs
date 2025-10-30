// run-pass
// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

#[derive(Debug)]
struct S<const N: usize>;

fn main() {
    assert_eq!(std::any::type_name::<S<3>>(), "const_generic_type_name::S<3>");
}

