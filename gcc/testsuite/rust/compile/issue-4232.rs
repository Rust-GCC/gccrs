// { dg-options "-w" }
#[repr(C)] // { dg-error "attribute should be applied to a struct, enum, or union" }
fn a() {}