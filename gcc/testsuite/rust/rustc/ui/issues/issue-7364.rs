#![feature(box_syntax)]

use std::cell::RefCell;

// Regression test for issue 7364
static boxed: Box<RefCell<isize>> = box RefCell::new(0);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

fn main() { }

