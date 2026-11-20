// See also repr-transparent.rs

#[repr(transparent)] // { dg-error ".E0517." "" { target *-*-* } }
fn cant_repr_this() {}

#[repr(transparent)] // { dg-error ".E0517." "" { target *-*-* } }
static CANT_REPR_THIS: u32 = 0;

fn main() {}

