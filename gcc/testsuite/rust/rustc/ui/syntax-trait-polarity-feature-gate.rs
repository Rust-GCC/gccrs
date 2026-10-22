use std::marker::Send;

struct TestType;

trait TestTrait {}

impl !Send for TestType {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

