fn bar<T: Sized>() { }
fn foo<T: ?Sized>() { bar::<T>() }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
fn main() { }

