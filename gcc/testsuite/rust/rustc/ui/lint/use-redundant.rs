// check-pass
#![warn(unused_imports)]

use crate::foo::Bar;

mod foo {
    pub type Bar = i32;
}

fn baz() -> Bar {
    3
}

mod m1 { pub struct S {} }
mod m2 { pub struct S {} }

use m1::*; // { dg-warning "" "" { target *-*-* } }
use m2::*; // { dg-warning "" "" { target *-*-* } }

fn main() {
    use crate::foo::Bar; // { dg-warning "" "" { target *-*-* } }
    let _a: Bar = 3;
    baz();

    use m1::S;
    let _s = S {};
}

