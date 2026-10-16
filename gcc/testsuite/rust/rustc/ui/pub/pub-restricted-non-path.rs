#![feature(pub_restricted)]

pub (.) fn afn() {} // { dg-error "" "" { target *-*-* } }

fn main() {}

