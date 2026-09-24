mod foo { pub mod foo {  } }

use foo::foo;
// { dg-error ".E0255." "" { target *-*-* } .-1 }
// { dg-error ".E0255." "" { target *-*-* } .-2 }

fn main() {}

