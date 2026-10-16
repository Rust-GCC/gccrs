#![feature(non_ascii_idents)]

pub fn main () {}

fn საჭმელად_გემრიელი_სადილი ( ) -> isize { // { dg-error ".E0308." "" { target *-*-* } }
}

