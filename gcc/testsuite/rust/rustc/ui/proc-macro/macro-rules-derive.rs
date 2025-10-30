// aux-build:first-second.rs

extern crate first_second;
use first_second::*;

macro_rules! produce_it {
    ($name:ident) => {
        #[first]
        struct $name {
            field: MissingType // { dg-error ".E0412." "" { target *-*-* } }
        }
    }
}

produce_it!(MyName);

fn main() {
    println!("Hello, world!");
}

