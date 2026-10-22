// Check that when you implement a trait that has a sized type
// parameter, the corresponding value must be sized. Also that the
// self type must be sized if appropriate.

trait Foo<T> : Sized { fn take(self, x: &T) { } } // Note: T is sized

impl Foo<[isize]> for usize { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

impl Foo<isize> for [usize] { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

pub fn main() { }

