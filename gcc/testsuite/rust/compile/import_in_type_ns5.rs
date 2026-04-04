#![feature(no_core)]
#![no_core]

mod inner {
    mod cmp {
        enum Ordering {
            Greater,
            Less,
        }
    }
}

use inner::cmp;

fn foo() /* { dg-warning "function is never used" } */
{
    use cmp::Ordering::Greater;
    use cmp::Ordering::Less;
}
