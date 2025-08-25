#![feature(rustc_attrs)]

// Regression test for #56877. We want to ensure that the presence of
// `PhantomData` does not prevent `Bar` from being considered a
// homogeneous aggregate.

#[repr(C)]
pub struct BaseCase {
    pub a: f32,
    pub b: f32,
}

#[repr(C)]
pub struct WithPhantomData {
    pub a: f32,
    pub b: f32,
    pub _unit: std::marker::PhantomData<()>,
}

pub struct EmptyRustStruct {
}

#[repr(C)]
pub struct WithEmptyRustStruct {
    pub a: f32,
    pub b: f32,
    pub _unit: EmptyRustStruct,
}

pub struct TransitivelyEmptyRustStruct {
    field: EmptyRustStruct,
    array: [u32; 0],
}

#[repr(C)]
pub struct WithTransitivelyEmptyRustStruct {
    pub a: f32,
    pub b: f32,
    pub _unit: TransitivelyEmptyRustStruct,
}

pub enum EmptyRustEnum {
    Dummy,
}

#[repr(C)]
pub struct WithEmptyRustEnum {
    pub a: f32,
    pub b: f32,
    pub _unit: EmptyRustEnum,
}

#[rustc_layout(homogeneous_aggregate)]
pub type Test1 = BaseCase;
// { dg-error "" "" { target *-*-* } .-1 }

#[rustc_layout(homogeneous_aggregate)]
pub type Test2 = WithPhantomData;
// { dg-error "" "" { target *-*-* } .-1 }

#[rustc_layout(homogeneous_aggregate)]
pub type Test3 = WithEmptyRustStruct;
// { dg-error "" "" { target *-*-* } .-1 }

#[rustc_layout(homogeneous_aggregate)]
pub type Test4 = WithTransitivelyEmptyRustStruct;
// { dg-error "" "" { target *-*-* } .-1 }

#[rustc_layout(homogeneous_aggregate)]
pub type Test5 = WithEmptyRustEnum;
// { dg-error "" "" { target *-*-* } .-1 }

fn main() { }

