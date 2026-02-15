#![feature(type_alias_impl_trait)]

type Foo<T> = impl Default;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

#[allow(unused)]
fn foo<T: Default>(t: T) -> Foo<T> {
    t
}

struct NotDefault;

fn main() {
    let _ = Foo::<NotDefault>::default();
}

