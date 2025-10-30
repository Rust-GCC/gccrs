// Regression test for issue #20533. At some point, only 1 out of the
// 3 errors below were being reported.

use std::marker::PhantomData;

fn foo<'a, T>(_x: &'a T) -> PhantomData<&'a ()> {
    PhantomData
}

struct Wrap<T>(T);

fn bar<'a, T>(_x: &'a T) -> Wrap<PhantomData<&'a ()>> {
    Wrap(PhantomData)
}

struct Baked<'a>(PhantomData<&'a ()>);

fn baz<'a, T>(_x: &'a T) -> Baked<'a> {
    Baked(PhantomData)
}

struct AffineU32(u32);

fn main() {
    {
        let a = AffineU32(1);
        let x = foo(&a);
        drop(a); // { dg-error ".E0505." "" { target *-*-* } }
        drop(x);
    }
    {
        let a = AffineU32(1);
        let x = bar(&a);
        drop(a); // { dg-error ".E0505." "" { target *-*-* } }
        drop(x);
    }
    {
        let a = AffineU32(1);
        let x = baz(&a);
        drop(a); // { dg-error ".E0505." "" { target *-*-* } }
        drop(x);
    }
}

