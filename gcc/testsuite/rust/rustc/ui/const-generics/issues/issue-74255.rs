// [full] check-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

#[derive(PartialEq, Eq)]
enum IceEnum {
    Variant
}

struct IceStruct;

impl IceStruct {
    fn ice_struct_fn<const I: IceEnum>() {}
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    IceStruct::ice_struct_fn::<{IceEnum::Variant}>();
}

