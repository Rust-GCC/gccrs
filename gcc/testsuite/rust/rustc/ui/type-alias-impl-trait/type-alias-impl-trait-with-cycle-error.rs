#![feature(type_alias_impl_trait)]

type Foo = impl Fn() -> Foo;
// { dg-error "" "" { target *-*-* } .-1 }

fn crash(x: Foo) -> Foo {
    x
}

fn main() {

}

