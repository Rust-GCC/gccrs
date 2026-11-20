#![allow(warnings)]

struct Struct { a: usize }

const C: usize = 1;
static S: usize = 1;

const T1: &'static usize = &C;
const T2: &'static usize = &S; // { dg-error ".E0013." "" { target *-*-* } }
static T3: &'static usize = &C;
static T4: &'static usize = &S;

const T5: usize = C;
const T6: usize = S; // { dg-error ".E0013." "" { target *-*-* } }
static T7: usize = C;
static T8: usize = S;

const T9: Struct = Struct { a: C };
const T10: Struct = Struct { a: S };
// { dg-error ".E0013." "" { target *-*-* } .-1 }
static T11: Struct = Struct { a: C };
static T12: Struct = Struct { a: S };

fn main() {}

