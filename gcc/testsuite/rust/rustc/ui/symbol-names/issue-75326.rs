// build-fail
// ignore-tidy-linelength
// revisions: legacy v0
//[legacy]compile-flags: -Z symbol-mangling-version=legacy
//[v0]compile-flags: -Z symbol-mangling-version=v0
//[legacy]normalize-stderr-32bit: "h[\d\w]+" -> "SYMBOL_HASH"
//[legacy]normalize-stderr-64bit: "h[\d\w]+" -> "SYMBOL_HASH"

#![feature(rustc_attrs)]

pub(crate) struct Foo<I, E>(I, E);

pub trait Iterator2 {
    type Item;

    fn next(&mut self) -> Option<Self::Item>;

    fn find<P>(&mut self, predicate: P) -> Option<Self::Item>
    where
        Self: Sized,
        P: FnMut(&Self::Item) -> bool,
    {
        unimplemented!()
    }
}

struct Bar;

impl Iterator2 for Bar {
    type Item = (u32, u16);

    fn next(&mut self) -> Option<Self::Item> {
        unimplemented!()
    }
}

impl<I, T, E> Iterator2 for Foo<I, E>
where
    I: Iterator2<Item = (T, E)>,
{
    type Item = T;

    #[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    fn next(&mut self) -> Option<Self::Item> {
        self.find(|_| true)
    }
}

fn main() {
    let mut a = Foo(Bar, 1u16);
    let _ = a.next();
}

