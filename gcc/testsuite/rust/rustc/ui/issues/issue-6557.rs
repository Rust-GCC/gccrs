// check-pass
#![allow(dead_code)]
// pretty-expanded FIXME #23616

#![feature(box_patterns)]
#![feature(box_syntax)]

fn foo(box (_x, _y): Box<(isize, isize)>) {}

pub fn main() {}

