// This tests that conflicting imports shows both `use` lines
// when reporting the error.

mod sub1 {
    pub fn foo() {} // implementation 1
}

mod sub2 {
    pub fn foo() {} // implementation 2
}

use sub1::foo;
use sub2::foo; // { dg-error ".E0252." "" { target *-*-* } }

fn main() {}

