// The purpose of this test is to demonstrate that trait alias expansion
// preserves the rule that `dyn Trait` may only reference one non-auto trait.

#![feature(trait_alias)]

use std::marker::Unpin;

// Some arbitrary object-safe traits:
trait ObjA {}
trait ObjB {}

// Nest a few levels deep:
trait _0 = ObjA;
trait _1 = _0;

type _T00 = dyn _0 + ObjB;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T01 = dyn ObjB + _0;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T02 = dyn ObjB + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T03 = dyn _1 + ObjB;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Nest some more and in weird ways:

trait _2 = ObjB;
trait _3 = _2;
trait _4 = _3;

type _T10 = dyn _2 + _3;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T11 = dyn _3 + _2;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T12 = dyn _2 + _4;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T13 = dyn _4 + _2;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Include auto traits:

trait _5 = Sync + ObjB + Send;

type _T20 = dyn _5 + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T21 = dyn _1 + _5;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T22 = dyn _5 + ObjA;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T23 = dyn ObjA + _5;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T24 = dyn Send + _5 + _1 + Sync;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T25 = dyn _1 + Sync + _5 + Send;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T26 = dyn Sync + Send + _5 + ObjA;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T27 = dyn Send + Sync + ObjA + _5;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Also nest:

trait _6 = _1 + _5;
trait _7 = _6;
trait _8 = _7;

type _T30 = dyn _6;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T31 = dyn _6 + Send;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T32 = dyn Send + _6;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T33 = dyn _8;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T34 = dyn _8 + Send;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T35 = dyn Send + _8;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

// Nest some more:

trait _9 = _5 + Sync;
trait _10 = Unpin + _9;

type _T40 = dyn _10 + ObjA;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T41 = dyn ObjA + _10;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T42 = dyn _10 + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T43 = dyn Send + _10 + Sync + ObjA;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T44 = dyn ObjA + _10 + Send + Sync;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

type _T45 = dyn Sync + Send + _10 + _1;
// { dg-error ".E0225." "" { target *-*-* } .-1 }

fn main() {}

