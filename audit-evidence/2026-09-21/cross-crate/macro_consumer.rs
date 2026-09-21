#![feature(no_core)]
#![no_core]
#[macro_use] extern crate macro_producer;
fn main()->i32 { answer!() - 42 }
