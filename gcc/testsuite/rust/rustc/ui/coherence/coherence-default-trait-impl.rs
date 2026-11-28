#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

auto trait MySafeTrait {}

struct Foo;

unsafe impl MySafeTrait for Foo {}
// { dg-error ".E0199." "" { target *-*-* } .-1 }

unsafe auto trait MyUnsafeTrait {}

impl MyUnsafeTrait for Foo {}
// { dg-error ".E0200." "" { target *-*-* } .-1 }

fn main() {}

