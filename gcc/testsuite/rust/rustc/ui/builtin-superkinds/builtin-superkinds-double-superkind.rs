// Test for traits that inherit from multiple builtin kinds at once,
// testing that all such kinds must be present on implementing types.

trait Foo : Send+Sync { }

impl <T: Sync+'static> Foo for (T,) { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

impl <T: Send> Foo for (T,T) { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

impl <T: Send+Sync> Foo for (T,T,T) { } // (ok)

fn main() { }

