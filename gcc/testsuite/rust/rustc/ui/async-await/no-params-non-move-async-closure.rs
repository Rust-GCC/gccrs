// edition:2018

#![feature(async_closure)]

fn main() {
    let _ = async |x: u8| {};
// { dg-error ".E0708." "" { target *-*-* } .-1 }
}

