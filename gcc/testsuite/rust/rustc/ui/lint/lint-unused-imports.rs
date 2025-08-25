#![deny(unused_imports)]
#![allow(dead_code)]

use bar::c::cc as cal;

use std::mem::*;            // shouldn't get errors for not using
                            // everything imported
use std::fmt::{};
// { dg-error "" "" { target *-*-* } .-1 }

// Should get errors for both 'Some' and 'None'
use std::option::Option::{Some, None};
// { dg-error "" "" { target *-*-* } .-1 }

use test::A;       // { dg-error "" "" { target *-*-* } }
// Be sure that if we just bring some methods into scope that they're also
// counted as being used.
use test::B;
// But only when actually used: do not get confused by the method with the same name.
use test::B2; // { dg-error "" "" { target *-*-* } }

// Make sure this import is warned about when at least one of its imported names
// is unused
use test2::{foo, bar}; // { dg-error "" "" { target *-*-* } }

mod test2 {
    pub fn foo() {}
    pub fn bar() {}
}

mod test {
    pub trait A { fn a(&self) {} }
    pub trait B { fn b(&self) {} }
    pub trait B2 { fn b(&self) {} }
    pub struct C;
    impl A for C {}
    impl B for C {}
}

mod foo {
    pub struct Point{pub x: isize, pub y: isize}
    pub struct Square{pub p: Point, pub h: usize, pub w: usize}
}

mod bar {
    // Don't ignore on 'pub use' because we're not sure if it's used or not
    pub use std::cmp::PartialEq;
    pub struct Square;

    pub mod c {
        use foo::Point;
        use foo::Square; // { dg-error "" "" { target *-*-* } }
        pub fn cc(_p: Point) -> super::Square {
            fn f() -> super::Square {
                super::Square
            }
            f()
        }
    }

    #[allow(unused_imports)]
    mod foo {
        use std::cmp::PartialEq;
    }
}

fn g() {
    use self::g; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    fn f() {
        self::g();
    }
}

// cf. issue #35135.
#[allow(unused_variables)]
fn h() {
    use test2::foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    let foo = 0;
}

fn main() {
    cal(foo::Point{x:3, y:9});
    let mut a = 3;
    let mut b = 4;
    swap(&mut a, &mut b);
    test::C.b();
    let _a = foo();
}

