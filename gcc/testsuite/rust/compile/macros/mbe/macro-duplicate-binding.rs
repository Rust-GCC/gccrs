#![feature(no_core)]
#![no_core]

macro_rules! foo { // { dg-warning "unused macro definition" }
    ($a:ident, $a:ident) => {0} // { dg-error "duplicate matcher binding" }
}

fn main() {}
