// check-pass
#![allow(dead_code)]
// pretty-expanded FIXME #23616

fn foo<T: ::std::cmp::PartialEq>(_t: T) { }

pub fn main() { }

