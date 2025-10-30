// The purpose of this test is to demonstrate that duplicating object safe traits
// that are not auto traits is rejected with trait aliases even though one could
// reasonably accept this.

#![feature(trait_alias)]

use std::marker::Unpin;

// Some arbitrary object-safe trait:
trait Obj {}

// Nest a few levels deep:
trait _0 = Obj;
trait _1 = _0;

type _T00 = dyn _0 + _0;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T01 = dyn _1 + _0;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T02 = dyn _1 + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T03 = dyn Obj + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T04 = dyn _1 + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Nest some more and in weird ways:

trait _2 = _0 + _1;
trait _3 = Obj;
trait _4 = _3;

type _T10 = dyn _2 + _3;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T11 = dyn _3 + _2;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T12 = dyn Obj + _2;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T13 = dyn _2 + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T14 = dyn _1 + _3;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T15 = dyn _3 + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T16 = dyn _1 + _4;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T17 = dyn _4 + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Include auto traits:

trait _5 = Obj + Send;

type _T20 = dyn _5 + _5;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T21 = dyn Obj + _5;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T22 = dyn _5 + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T23 = dyn _5 + Send + Sync + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Also nest:

trait _6 = _5 + _5; // ==> Obj + Send + Obj + Send

type _T30 = dyn _6;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T31 = dyn _6 + Send;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T32 = dyn Send + _6;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Nest some more:

trait _7 = _5 + Sync;
trait _8 = Unpin + _7;

type _T40 = dyn _8 + Obj;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T41 = dyn Obj + _8;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T42 = dyn _8 + _4;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T43 = dyn _4 + _8;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T44 = dyn _4 + Send + Sync + _8;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Take higher ranked types into account.

// Note that `'a` and `'b` are intentionally different to make sure we consider
// them semantically the same.
trait ObjL<'l> {}
trait _9 = for<'a> ObjL<'a>;
trait _10 = for<'b> ObjL<'b>;
type _T50 = dyn _9 + _10;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

trait ObjT<T> {}
trait _11 = ObjT<for<'a> fn(&'a u8)>;
trait _12 = ObjT<for<'b> fn(&'b u8)>;
type _T60 = dyn _11 + _12;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

fn main() {}

