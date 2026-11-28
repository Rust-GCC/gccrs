#![feature(no_core)]
#![no_core]

use self::Ordering::*;
use Ordering::*;

pub enum Ordering {
    A,
    B,
}

fn foo(_: Ordering) {}

fn main() {
    let a = A;

    foo(a);
    foo(B);
}
