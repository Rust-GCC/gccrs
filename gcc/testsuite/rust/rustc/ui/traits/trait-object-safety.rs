// Check that static methods are not object-safe.

trait Tr {
    fn foo();
    fn bar(&self) { }
}

struct St;

impl Tr for St {
    fn foo() {}
}

fn main() {
    let _: &dyn Tr = &St; // { dg-error ".E0038." "" { target *-*-* } }
// { dg-error ".E0038." "" { target *-*-* } .-1 }
}

