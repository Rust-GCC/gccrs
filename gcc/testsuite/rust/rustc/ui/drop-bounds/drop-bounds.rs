#![deny(drop_bounds)]
fn foo<T: Drop>() {} // { dg-error "" "" { target *-*-* } }
fn bar<U>()
where
    U: Drop, // { dg-error "" "" { target *-*-* } }
{
}
fn baz(_x: impl Drop) {} // { dg-error "" "" { target *-*-* } }
struct Foo<T: Drop> { // { dg-error "" "" { target *-*-* } }
  _x: T
}
struct Bar<U> where U: Drop { // { dg-error "" "" { target *-*-* } }
  _x: U
}
trait Baz: Drop { // { dg-error "" "" { target *-*-* } }
}
impl<T: Drop> Baz for T { // { dg-error "" "" { target *-*-* } }
}
fn main() {}

