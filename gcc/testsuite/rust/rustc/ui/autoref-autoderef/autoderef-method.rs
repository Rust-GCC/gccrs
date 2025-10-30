// run-pass
#![allow(non_camel_case_types)]
#![feature(box_syntax)]

trait double {
    fn double(self: Box<Self>) -> usize;
}

impl double for usize {
    fn double(self: Box<usize>) -> usize { *self * 2 }
}

pub fn main() {
    let x: Box<_> = box 3;
    assert_eq!(x.double(), 6);
}

