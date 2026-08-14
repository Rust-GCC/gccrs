mod foo {
    pub struct A;
    pub struct B;
}

use foo::{self};
// { dg-error ".E0255." "" { target *-*-* } .-1 }

use foo as self;
// { dg-error "" "" { target *-*-* } .-1 }

use foo::self; // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error ".E0255." "" { target *-*-* } .-1 }

use foo::A;
use foo::{self as A};
// { dg-error ".E0252." "" { target *-*-* } .-1 }

fn main() {}

