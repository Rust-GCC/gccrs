// Test `ignored_generic_bounds` lint warning about bounds in type aliases.

// check-pass
#![allow(dead_code)]

use std::rc::Rc;

type SVec<T: Send + Send> = Vec<T>;
// { dg-warning "" "" { target *-*-* } .-1 }
type S2Vec<T> where T: Send = Vec<T>;
// { dg-warning "" "" { target *-*-* } .-1 }
type VVec<'b, 'a: 'b + 'b> = (&'b u32, Vec<&'a i32>);
// { dg-warning "" "" { target *-*-* } .-1 }
type WVec<'b, T: 'b + 'b> = (&'b u32, Vec<T>);
// { dg-warning "" "" { target *-*-* } .-1 }
type W2Vec<'b, T> where T: 'b, T: 'b = (&'b u32, Vec<T>);
// { dg-warning "" "" { target *-*-* } .-1 }

static STATIC: u32 = 0;

fn foo<'a>(y: &'a i32) {
    // If any of the bounds above would matter, the code below would be rejected.
    // This can be seen when replacing the type aliases above by newtype structs.
    // (The type aliases have no unused parameters to make that a valid transformation.)
    let mut x: SVec<_> = Vec::new();
    x.push(Rc::new(42)); // is not send

    let mut x: S2Vec<_> = Vec::new();
    x.push(Rc::new(42)); // is not `Send`

    let mut x: VVec<'static, 'a> = (&STATIC, Vec::new());
    x.1.push(y); // `'a: 'static` does not hold

    let mut x: WVec<'static, &'a i32> = (&STATIC, Vec::new());
    x.1.push(y); // `&'a i32: 'static` does not hold

    let mut x: W2Vec<'static, &'a i32> = (&STATIC, Vec::new());
    x.1.push(y); // `&'a i32: 'static` does not hold
}

// Bounds are not checked either; i.e., the definition is not necessarily well-formed.
struct Sendable<T: Send>(T);
type MySendable<T> = Sendable<T>; // no error here!

// However, bounds *are* taken into account when accessing associated types
trait Bound { type Assoc; }
type T1<U: Bound> = U::Assoc; // { dg-warning "" "" { target *-*-* } }
type T2<U> where U: Bound = U::Assoc;  // { dg-warning "" "" { target *-*-* } }

// This errors:
// `type T3<U> = U::Assoc;`
// Do this instead:
type T4<U> = <U as Bound>::Assoc;

// Make sure the help about associatd types is not shown incorrectly
type T5<U: Bound> = <U as Bound>::Assoc;  // { dg-warning "" "" { target *-*-* } }
type T6<U: Bound> = ::std::vec::Vec<U>;  // { dg-warning "" "" { target *-*-* } }

fn main() {}

