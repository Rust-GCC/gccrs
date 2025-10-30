fn is_sized<T:Sized>() { }
fn not_sized<T: ?Sized>() { }

enum Foo<U> { FooSome(U), FooNone }
fn foo1<T>() { not_sized::<Foo<T>>() } // Hunky dory.
fn foo2<T: ?Sized>() { not_sized::<Foo<T>>() }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
//
// Not OK: `T` is not sized.

fn main() { }

