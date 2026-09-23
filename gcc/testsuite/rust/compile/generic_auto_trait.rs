#![feature(no_core)]
#![no_core]

#![feature(auto_traits)]
auto trait IsCooler<G> {}
// { dg-error "auto traits cannot have generic parameters .E0567." "" { target *-*-* } .-1 }
