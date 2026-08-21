// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub trait Foo {}
pub trait Bar {}

pub fn a(_x: &Foo) {}
// { dg-warning "trait objects without an explicit .dyn. are deprecated" "" { target *-*-* } .-1 }

pub fn b(_x: &dyn Foo) {}

pub fn c(_x: &(Foo + Bar)) {}
// { dg-warning "trait objects without an explicit .dyn. are deprecated" "" { target *-*-* } .-1 }
