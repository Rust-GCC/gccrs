#![feature(isa_attribute)]

#[instruction_set()] // { dg-error ".E0778." "" { target *-*-* } }
fn no_isa_defined() {
}

fn main() {
}

