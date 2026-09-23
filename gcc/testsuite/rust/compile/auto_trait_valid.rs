// #![feature(auto_traits)] // not present in Rust 1.49 yet
#![feature(no_core)]
#![no_core]


#![feature(auto_traits)]

auto trait MegaSend {}
pub auto trait MegaSync {}
unsafe auto trait SuperSync {}
pub unsafe auto trait SuperSend {}

fn main() {}
