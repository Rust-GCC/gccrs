// Regression test for https://github.com/rust-lang/rust/issues/56445#issuecomment-518402995.
// revisions: full min
#![cfg_attr(full, feature(const_generics))] // { dg-warning "" "" { target *-*-* } }
#![cfg_attr(min, feature(min_const_generics))]
#![crate_type = "lib"]

use std::marker::PhantomData;

struct Bug<'a, const S: &'a str>(PhantomData<&'a ()>);
// { dg-error "" "" { target *-*-* } .-1 }

impl Bug<'_, ""> {}

