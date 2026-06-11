#![feature(no_core)]
#![no_core]

pub enum E {
    A = {
        pub enum F {}
        0
    },
}

fn main() {}
