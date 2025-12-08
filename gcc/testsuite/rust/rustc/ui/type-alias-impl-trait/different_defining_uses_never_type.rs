#![feature(type_alias_impl_trait)]

fn main() {}

// two definitions with different types
type Foo = impl std::fmt::Debug;

fn foo() -> Foo {
    ""
}

fn bar() -> Foo { // { dg-error "" "" { target *-*-* } }
    panic!()
}

fn boo() -> Foo { // { dg-error "" "" { target *-*-* } }
    loop {}
}

