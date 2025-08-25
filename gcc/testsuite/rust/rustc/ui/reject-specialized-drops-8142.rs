// Issue 8142: Test that Drop impls cannot be specialized beyond the
// predicates attached to the type definition itself.

trait Bound { fn foo(&self) { } }
struct K<'l1,'l2> { x: &'l1 i8, y: &'l2 u8 }
struct L<'l1,'l2> { x: &'l1 i8, y: &'l2 u8 }
struct M<'m> { x: &'m i8 }
struct N<'n> { x: &'n i8 }
struct O<To> { x: *const To }
struct P<Tp> { x: *const Tp }
struct Q<Tq> { x: *const Tq }
struct R<Tr> { x: *const Tr }
struct S<Ts:Bound> { x: *const Ts }
struct T<'t,Ts:'t> { x: &'t Ts }
struct U;
struct V<Tva, Tvb> { x: *const Tva, y: *const Tvb }
struct W<'l1, 'l2> { x: &'l1 i8, y: &'l2 u8 }

enum Enum<T> { Variant(T) }
struct TupleStruct<T>(T);
union Union<T: Copy> { f: T }

impl<'al,'adds_bnd:'al> Drop for K<'al,'adds_bnd> {                        // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }
    fn drop(&mut self) { } }

impl<'al,'adds_bnd>     Drop for L<'al,'adds_bnd> where 'adds_bnd:'al {    // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }
    fn drop(&mut self) { } }

impl<'ml>               Drop for M<'ml>         { fn drop(&mut self) { } } // ACCEPT

impl                    Drop for N<'static>     { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }

impl<COkNoBound> Drop for O<COkNoBound> { fn drop(&mut self) { } } // ACCEPT

impl              Drop for P<i8>          { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0366." "" { target *-*-* } .-1 }

impl<AddsBnd:Bound> Drop for Q<AddsBnd> { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }

impl<'rbnd,AddsRBnd:'rbnd> Drop for R<AddsRBnd> { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }

impl<Bs:Bound>    Drop for S<Bs>          { fn drop(&mut self) { } } // ACCEPT

impl<'t,Bt:'t>    Drop for T<'t,Bt>       { fn drop(&mut self) { } } // ACCEPT

impl              Drop for U              { fn drop(&mut self) { } } // ACCEPT

impl<One>         Drop for V<One,One>     { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0366." "" { target *-*-* } .-1 }

impl<'lw>         Drop for W<'lw,'lw>     { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0495." "" { target *-*-* } .-1 }

impl<AddsBnd:Bound> Drop for Enum<AddsBnd> { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }

impl<AddsBnd:Bound> Drop for TupleStruct<AddsBnd> { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }

impl<AddsBnd:Copy + Bound> Drop for Union<AddsBnd> { fn drop(&mut self) { } } // REJECT
// { dg-error ".E0367." "" { target *-*-* } .-1 }

pub fn main() { }

