#![feature(type_alias_impl_trait)]
#![feature(impl_trait_in_bindings)]
#![allow(incomplete_features)]

type FooArg<'a> = &'a dyn ToString;
type FooRet = impl std::fmt::Debug;

type FooItem = Box<dyn Fn(FooArg) -> FooRet>;
type Foo = impl Iterator<Item = FooItem>; // { dg-error ".E0271." "" { target *-*-* } }

#[repr(C)]
struct Bar(u8);

impl Iterator for Bar {
    type Item = FooItem;

    fn next(&mut self) -> Option<Self::Item> {
        Some(Box::new(quux))
    }
}

fn quux(st: FooArg) -> FooRet {
    Some(st.to_string())
}

fn ham() -> Foo {
    Bar(1)
}

fn oof() -> impl std::fmt::Debug {
    let mut bar = ham();
    let func = bar.next().unwrap();
    return func(&"oof");
}

fn main() {
    let _ = oof();
}

