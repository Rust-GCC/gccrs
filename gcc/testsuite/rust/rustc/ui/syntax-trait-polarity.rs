#![feature(negative_impls)]

use std::marker::Send;

struct TestType;

impl !TestType {}
// { dg-error "" "" { target *-*-* } .-1 }

trait TestTrait {}

unsafe impl !Send for TestType {}
// { dg-error ".E0198." "" { target *-*-* } .-1 }
impl !TestTrait for TestType {}

struct TestType2<T>(T);

impl<T> !TestType2<T> {}
// { dg-error "" "" { target *-*-* } .-1 }

unsafe impl<T> !Send for TestType2<T> {}
// { dg-error ".E0198." "" { target *-*-* } .-1 }
impl<T> !TestTrait for TestType2<T> {}

fn main() {}

