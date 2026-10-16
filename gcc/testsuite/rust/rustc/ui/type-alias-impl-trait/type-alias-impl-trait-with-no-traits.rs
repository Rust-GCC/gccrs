#![feature(type_alias_impl_trait)]

type Foo = impl 'static;
// { dg-error "" "" { target *-*-* } .-1 }

fn foo() -> Foo {
    "foo"
}

fn bar() -> impl 'static { // { dg-error "" "" { target *-*-* } }
    "foo"
}

fn main() {}

