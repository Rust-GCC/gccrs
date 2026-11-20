// aux-build:go_trait.rs

extern crate go_trait;

use go_trait::{Go,GoMut};
use std::fmt::Debug;
use std::default::Default;

struct MyThingy;

impl Go for MyThingy {
    fn go(&self, arg: isize) { }
}

impl GoMut for MyThingy {
// { dg-error ".E0119." "" { target *-*-* } .-1 }
    fn go_mut(&mut self, arg: isize) { }
}

fn main() { }

