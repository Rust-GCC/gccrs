// aux-build:foreign-crate.rs
#![feature(type_alias_impl_trait)]

extern crate foreign_crate;

trait LocalTrait {}
impl<T> LocalTrait for foreign_crate::ForeignType<T> {}

type AliasOfForeignType<T> = impl LocalTrait;
fn use_alias<T>(val: T) -> AliasOfForeignType<T> {
    foreign_crate::ForeignType(val)
}

impl<T> foreign_crate::ForeignTrait for AliasOfForeignType<T> {}
// { dg-error ".E0207." "" { target *-*-* } .-1 }

fn main() {}

