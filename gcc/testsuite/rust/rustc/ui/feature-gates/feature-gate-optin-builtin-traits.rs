// Test that default and negative trait implementations are gated by
// `optin_builtin_traits` feature gate

struct DummyStruct;

auto trait AutoDummyTrait {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

impl !AutoDummyTrait for DummyStruct {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

