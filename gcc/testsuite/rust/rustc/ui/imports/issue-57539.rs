// edition:2018

mod core {
    use core; // { dg-error ".E0659." "" { target *-*-* } }
    use crate::*;
}

fn main() {}

