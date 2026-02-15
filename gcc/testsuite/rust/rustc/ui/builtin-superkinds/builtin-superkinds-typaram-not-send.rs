// Basic test for traits inheriting from the builtin kinds.

trait Foo : Send { }

impl <T: Sync+'static> Foo for T { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() { }

