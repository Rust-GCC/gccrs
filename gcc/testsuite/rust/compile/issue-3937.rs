#![deny(dead_code)]
#![feature(no_core)]
#![no_core]

enum Foo {
    Bar,
}
fn main() {
    let p = [0; 0];
    p.bar(); // { dg-error "no method named .bar. found in the current scope" }
}

trait Bar {}

impl Foo for [u32; Foo::Bar as usize] {} // { dg-error "Expected a trait found .Foo." }