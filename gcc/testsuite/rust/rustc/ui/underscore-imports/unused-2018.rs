// edition:2018

#![deny(unused_imports)]

mod multi_segment {
    use core::any; // { dg-error "" "" { target *-*-* } }
}

mod single_segment {
    use core; // { dg-error "" "" { target *-*-* } }
}

mod single_segment_underscore {
    use core as _; // OK
}

fn main() {}

