#![allow(unused_macros)]

macro_rules! assign {
    (($($a:tt)*) = ($($b:tt))*) => { // { dg-error "" "" { target *-*-* } }
        $($a)* = $($b)*
    }
}

fn main() {}

