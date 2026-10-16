#![allow(unused)]

macro_rules! m {
    ($attr_path: path) => {
        #[$attr_path]
        fn f() {}
    }
}

m!(inline<u8>); // { dg-error "" "" { target *-*-* } }

fn main() {}

