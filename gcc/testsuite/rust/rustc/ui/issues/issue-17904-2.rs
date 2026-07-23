// Test that we can parse a unit struct with a where clause, even if
// it leads to an error later on since `T` is unused.

struct Foo<T> where T: Copy; // { dg-error ".E0392." "" { target *-*-* } }

fn main() {}

