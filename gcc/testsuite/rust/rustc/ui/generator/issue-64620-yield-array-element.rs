// Regression test for #64620

#![feature(generators)]

pub fn crash(arr: [usize; 1]) {
    yield arr[0]; // { dg-error ".E0627." "" { target *-*-* } }
}

fn main() {}

