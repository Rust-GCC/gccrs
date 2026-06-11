#![feature(no_core)]
#![no_core]

pub enum Foo<'a> {}

pub enum Bar<'a> {
    in_band_def_explicit_impl(Foo<'a>),
}
