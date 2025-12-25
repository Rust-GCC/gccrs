// { dg-options "-w" }
#[repr(C)] // { dg-error "the ...repr.. attribute may only be applied to structs, enums and unions" }
fn a() {}
