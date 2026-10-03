// compile-flags: --test

#![feature(test)]

extern crate test;
use std::num::ParseIntError;
use test::Bencher;

#[test]
#[should_panic]
fn not_a_num() -> Result<(), ParseIntError> {
// { dg-error "" "" { target *-*-* } .-1 }
    let _: u32 = "abc".parse()?;
    Ok(())
}

