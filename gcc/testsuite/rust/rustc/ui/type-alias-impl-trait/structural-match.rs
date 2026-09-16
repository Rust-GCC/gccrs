#![feature(const_impl_trait, type_alias_impl_trait)]

type Foo = impl Send;

// This is not structural-match
struct A;

const fn value() -> Foo {
    A
}
const VALUE: Foo = value();

fn test() {
    match todo!() {
        VALUE => (),
// { dg-error "" "" { target *-*-* } .-1 }
        _ => (),
    }
}

fn main() {}

