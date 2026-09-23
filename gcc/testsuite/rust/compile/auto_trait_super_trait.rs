#![feature(no_core)]
#![no_core]

#![feature(auto_traits)]
trait Cold {}

auto trait IsCool: Cold {}
// { dg-error "auto traits cannot have super traits .E0568." "" { target *-*-* } .-1 }
