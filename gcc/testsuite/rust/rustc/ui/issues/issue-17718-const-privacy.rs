// aux-build:issue-17718-const-privacy.rs

extern crate issue_17718_const_privacy as other;

use a::B; // { dg-error ".E0603." "" { target *-*-* } }
use other::{
    FOO,
    BAR, // { dg-error ".E0603." "" { target *-*-* } }
    FOO2,
};

mod a {
    const B: usize = 3;
}

fn main() {}

