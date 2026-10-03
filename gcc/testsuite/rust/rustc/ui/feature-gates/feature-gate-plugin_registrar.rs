// Test that `#[plugin_registrar]` attribute is gated by `plugin_registrar`
// feature gate.

// the registration function isn't typechecked yet
#[plugin_registrar]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { dg-warning ".E0658." "" { target *-*-* } .-2 }
pub fn registrar() {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

