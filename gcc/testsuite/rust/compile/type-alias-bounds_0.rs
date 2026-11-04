// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

pub trait Foo {}

pub type Alias<T: Foo> = T;
// { dg-warning "bounds on generic parameters in type aliases are not enforced" "" { target *-*-* } .-1 }
