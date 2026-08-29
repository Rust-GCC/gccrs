// Make sure specialization cannot change impl polarity

#![feature(optin_builtin_traits)]
#![feature(negative_impls)]
#![feature(specialization)] // { dg-warning "" "" { target *-*-* } }

auto trait Foo {}

impl<T> Foo for T {}
impl !Foo for u8 {} // { dg-error ".E0751." "" { target *-*-* } }

auto trait Bar {}

impl<T> !Bar for T {}
impl Bar for u8 {} // { dg-error ".E0751." "" { target *-*-* } }

fn main() {}

