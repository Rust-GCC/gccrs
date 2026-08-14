#![deny(unused)]

#![feature(register_attr)]
#![feature(register_tool)]

#[register_attr(attr)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
#[register_tool(tool)] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {}

