// compile-flags: -Zsave-analysis
// This is also a regression test for #69415 and the above flag is needed.

#![feature(untagged_unions)]

trait Tr1 { type As1: Copy; }
trait Tr2 { type As2: Copy; }

struct S1;
#[derive(Copy, Clone)]
struct S2;
impl Tr1 for S1 { type As1 = S2; }

trait _Tr3 {
    type A: Iterator<Item: Copy>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    type B: Iterator<Item: 'static>;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

struct _St1<T: Tr1<As1: Tr2>> {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    outest: T,
    outer: T::As1,
    inner: <T::As1 as Tr2>::As2,
}

enum _En1<T: Tr1<As1: Tr2>> {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    Outest(T),
    Outer(T::As1),
    Inner(<T::As1 as Tr2>::As2),
}

union _Un1<T: Tr1<As1: Tr2>> {
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    outest: std::mem::ManuallyDrop<T>,
    outer: T::As1,
    inner: <T::As1 as Tr2>::As2,
}

type _TaWhere1<T> where T: Iterator<Item: Copy> = T;
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn _apit(_: impl Tr1<As1: Copy>) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn _apit_dyn(_: &dyn Tr1<As1: Copy>) {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn _rpit() -> impl Tr1<As1: Copy> { S1 }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn _rpit_dyn() -> Box<dyn Tr1<As1: Copy>> { Box::new(S1) }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const _cdef: impl Tr1<As1: Copy> = S1;
// { dg-error ".E0562." "" { target *-*-* } .-1 }
// { dg-error ".E0562." "" { target *-*-* } .-2 }
// FIXME: uncomment when `impl_trait_in_bindings` feature is fixed.
// const _cdef_dyn: &dyn Tr1<As1: Copy> = &S1;

static _sdef: impl Tr1<As1: Copy> = S1;
// { dg-error ".E0562." "" { target *-*-* } .-1 }
// { dg-error ".E0562." "" { target *-*-* } .-2 }
// FIXME: uncomment when `impl_trait_in_bindings` feature is fixed.
// static _sdef_dyn: &dyn Tr1<As1: Copy> = &S1;

fn main() {
    let _: impl Tr1<As1: Copy> = S1;
// { dg-error ".E0562." "" { target *-*-* } .-1 }
// { dg-error ".E0562." "" { target *-*-* } .-2 }
    // FIXME: uncomment when `impl_trait_in_bindings` feature is fixed.
    // let _: &dyn Tr1<As1: Copy> = &S1;
}

