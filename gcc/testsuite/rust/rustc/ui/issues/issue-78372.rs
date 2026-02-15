use std::ops::DispatchFromDyn; // { dg-error ".E0658." "" { target *-*-* } }
struct Smaht<T, MISC>(PhantomData); // { dg-error ".E0412." "" { target *-*-* } }
impl<T> DispatchFromDyn<Smaht<U, MISC>> for T {} // { dg-error ".E0210." "" { target *-*-* } }
// { dg-error ".E0210." "" { target *-*-* } .-1 }
// { dg-error ".E0210." "" { target *-*-* } .-2 }
// { dg-error ".E0210." "" { target *-*-* } .-3 }
// { dg-error ".E0210." "" { target *-*-* } .-4 }
trait Foo: X<u32> {}
trait X<T> {
    fn foo(self: Smaht<Self, T>);
}
trait Marker {}
impl Marker for dyn Foo {}
fn main() {}

