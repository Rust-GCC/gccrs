#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

// Test for issue #56934 - that it is impossible to redundantly
// implement an auto-trait for a trait object type that contains it.

// Positive impl variant.

auto trait Marker1 {}
auto trait Marker2 {}

trait Object: Marker1 {}

// A supertrait marker is illegal...
impl Marker1 for dyn Object + Marker2 { }   // { dg-error ".E0371." "" { target *-*-* } }
// ...and also a direct component.
impl Marker2 for dyn Object + Marker2 { }   // { dg-error ".E0371." "" { target *-*-* } }

// But implementing a marker if it is not present is OK.
impl Marker2 for dyn Object {} // OK

// A non-principal trait-object type is orphan even in its crate.
unsafe impl Send for dyn Marker2 {} // { dg-error ".E0117." "" { target *-*-* } }

// And impl'ing a remote marker for a local trait object is forbidden
// by one of these special orphan-like rules.
unsafe impl Send for dyn Object {} // { dg-error ".E0321." "" { target *-*-* } }
unsafe impl Send for dyn Object + Marker2 {} // { dg-error ".E0321." "" { target *-*-* } }

fn main() { }

