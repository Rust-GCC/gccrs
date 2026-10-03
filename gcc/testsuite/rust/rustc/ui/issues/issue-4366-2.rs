// ensures that 'use foo:*' doesn't import non-public item

use m1::*;

mod foo {
    pub fn foo() {}
}
mod a {
    pub mod b {
        use foo::foo;
        type Bar = isize;
    }
    pub mod sub {
        use a::b::*;
        fn sub() -> Bar { 1 }
// { dg-error ".E0412." "" { target *-*-* } .-1 }
    }
}

mod m1 {
    fn foo() {}
}

fn main() {
    foo(); // { dg-error ".E0423." "" { target *-*-* } }
}

