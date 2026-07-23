const X: u8 = 1;
static Y: u8 = 1;
fn foo() {}

impl X {}
// { dg-error ".E0573." "" { target *-*-* } .-1 }
impl Y {}
// { dg-error ".E0573." "" { target *-*-* } .-1 }
impl foo {}
// { dg-error ".E0573." "" { target *-*-* } .-1 }

fn main() {}

