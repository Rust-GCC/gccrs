// compile-flags: --crate-type lib
#![deny(missing_debug_implementations)]
#![allow(unused)]

use std::fmt;

pub enum A {} // { dg-error "" "" { target *-*-* } }

#[derive(Debug)]
pub enum B {}

pub enum C {}

impl fmt::Debug for C {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        Ok(())
    }
}

pub struct Foo; // { dg-error "" "" { target *-*-* } }

#[derive(Debug)]
pub struct Bar;

pub struct Baz;

impl fmt::Debug for Baz {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        Ok(())
    }
}

struct PrivateStruct;

enum PrivateEnum {}

#[derive(Debug)]
struct GenericType<T>(T);

