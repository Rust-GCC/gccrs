// revisions: min
// FIXME(const_generics): This test currently causes an ICE because
// we don't yet correctly deal with lifetimes, reenable this test once
// this is fixed.
#![cfg_attr(min, feature(min_const_generics))]

const fn foo<T>() -> usize { std::mem::size_of::<T>() }
const fn bar<const N: usize>() -> usize { N }
const fn faz<'a>(_: &'a ()) -> usize { 13 }
const fn baz<'a>(_: &'a ()) -> usize where &'a (): Sized { 13 }

struct Foo<const N: usize>;
fn test<'a, 'b, T, const N: usize>() where &'b (): Sized {
    let _: [u8; foo::<T>()]; // { dg-error "" "" { target *-*-* } }
    let _: [u8; bar::<N>()]; // { dg-error "" "" { target *-*-* } }
    let _: [u8; faz::<'a>(&())]; // { dg-error "" "" { target *-*-* } }
    let _: [u8; baz::<'a>(&())]; // { dg-error "" "" { target *-*-* } }
    let _: [u8; faz::<'b>(&())]; // { dg-error "" "" { target *-*-* } }
    let _: [u8; baz::<'b>(&())]; // { dg-error "" "" { target *-*-* } }

    // NOTE: This can be a future compat warning instead of an error,
    // so we stop compilation before emitting this error in this test.
    let _ = [0; foo::<T>()];

    let _ = [0; bar::<N>()]; // { dg-error "" "" { target *-*-* } }
    let _ = [0; faz::<'a>(&())]; // { dg-error "" "" { target *-*-* } }
    let _ = [0; baz::<'a>(&())]; // { dg-error "" "" { target *-*-* } }
    let _ = [0; faz::<'b>(&())]; // { dg-error "" "" { target *-*-* } }
    let _ = [0; baz::<'b>(&())]; // { dg-error "" "" { target *-*-* } }
    let _: Foo<{ foo::<T>() }>; // { dg-error "" "" { target *-*-* } }
    let _: Foo<{ bar::<N>() }>; // { dg-error "" "" { target *-*-* } }
    let _: Foo<{ faz::<'a>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _: Foo<{ baz::<'a>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _: Foo<{ faz::<'b>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _: Foo<{ baz::<'b>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _ = Foo::<{ foo::<T>() }>; // { dg-error "" "" { target *-*-* } }
    let _ = Foo::<{ bar::<N>() }>; // { dg-error "" "" { target *-*-* } }
    let _ = Foo::<{ faz::<'a>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _ = Foo::<{ baz::<'a>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _ = Foo::<{ faz::<'b>(&()) }>; // { dg-error "" "" { target *-*-* } }
    let _ = Foo::<{ baz::<'b>(&()) }>; // { dg-error "" "" { target *-*-* } }
}

fn main() {}

