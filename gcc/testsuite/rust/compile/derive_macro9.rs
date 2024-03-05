#![derive(Clone)]

#[derive(Clone)] // { dg-error "attribute can only be used on struct, union and enum declaration" }
pub fn foo() {}

pub struct Struct {
    #[derive(Clone)] // { dg-error "attribute can only be used on struct, union and enum declaration" }
    a: u32,
}

pub enum Enum {
    // FIXME: This is a bug, it should warn!
    #[derive(Clone)]
    A,
    // FIXME: This is a bug, it should warn!
    #[derive(Clone)]
    B(u32),
    // FIXME: This is a bug, it should warn!
    #[derive(Clone)]
    C { a: u32 },
}

pub union Union {
    #[derive(Clone)] // { dg-error "attribute can only be used on struct, union and enum declaration" }
    a: u32,
    b: u8,
}
