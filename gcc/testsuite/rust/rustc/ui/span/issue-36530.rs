// gate-test-custom_inner_attributes

#![feature(register_attr)]

#![register_attr(foo)]

#[foo]
mod foo {
    #![foo] // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

