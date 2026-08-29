fn is_sized<T:Sized>() { }
fn not_sized<T: ?Sized>() { }

struct Foo<T> { data: T }
fn foo1<T>() { not_sized::<Foo<T>>() } // Hunky dory.
fn foo2<T: ?Sized>() { not_sized::<Foo<T>>() }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
//
// Not OK: `T` is not sized.

struct Bar<T: ?Sized> { data: T }
fn bar1<T: ?Sized>() { not_sized::<Bar<T>>() }
fn bar2<T: ?Sized>() { is_sized::<Bar<T>>() }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
//
// Not OK: `Bar<T>` is not sized, but it should be.

fn main() { }

