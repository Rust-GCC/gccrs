#![feature(associated_type_bounds)]

struct Incorrect;

fn hello<F: for<'a> Iterator<Item: 'a>>() {
    Incorrect // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

