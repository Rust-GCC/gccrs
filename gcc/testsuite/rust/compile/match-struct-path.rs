#![feature(no_core)]
#![no_core]

pub struct S;

pub fn foo(v: S) {
    match v {
        S => ()
    }
}

pub fn bar(v: S) {
    match v {
        crate::S => ()
    }
}
