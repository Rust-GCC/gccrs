use std::iter::FromIterator;
use std::vec::IntoIter;
use std::ops::Add;

struct Foo<T, U: FromIterator<T>>(T, U);
struct WellFormed<Z = Foo<i32, i32>>(Z);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
struct WellFormedNoBounds<Z:?Sized = Foo<i32, i32>>(Z);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

struct Bounds<T:Copy=String>(T);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

struct WhereClause<T=String>(T) where T: Copy;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

trait TraitBound<T:Copy=String> {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

trait Super<T: Copy> { }
trait Base<T = String>: Super<T> { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

trait ProjectionPred<T:Iterator = IntoIter<i32>> where T::Item : Add<u8> {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() { }

