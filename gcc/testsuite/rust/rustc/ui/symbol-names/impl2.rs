// build-fail

#![feature(rustc_attrs)]
#![allow(dead_code)]

trait Foo {
    fn baz();
}

impl Foo for [u8; 1 + 2] {
    #[rustc_def_path] // { dg-error "" "" { target *-*-* } }
    fn baz() { }
}

fn main() {
}

