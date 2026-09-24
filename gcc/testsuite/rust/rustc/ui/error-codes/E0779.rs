#![feature(isa_attribute)]

#[instruction_set(arm::magic)] // { dg-error ".E0779." "" { target *-*-* } }
fn main() {

}

