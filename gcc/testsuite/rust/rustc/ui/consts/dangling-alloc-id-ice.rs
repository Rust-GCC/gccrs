// https://github.com/rust-lang/rust/issues/55223
#![allow(const_err)]

union Foo<'a> {
    y: &'a (),
    long_live_the_unit: &'static (),
}

const FOO: &() = {
// { dg-error "" "" { target *-*-* } .-1 }
    let y = ();
    unsafe { Foo { y: &y }.long_live_the_unit }
};

fn main() {}

