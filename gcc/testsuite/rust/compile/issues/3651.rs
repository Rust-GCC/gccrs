#![feature(no_core)]
#![no_core]
pub enum E {
    A = {
        enum F {
            Ai32 = -1,
        }
        0
    },
}

fn main() {}
