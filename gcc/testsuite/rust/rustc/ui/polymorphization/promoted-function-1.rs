// build-fail
// compile-flags: -Zpolymorphize=on
#![crate_type = "lib"]
#![feature(rustc_attrs)]

fn foo<'a>(_: &'a ()) {}

#[rustc_polymorphize_error]
pub fn test<T>() {
// { dg-error "" "" { target *-*-* } .-1 }
    foo(&());
}

