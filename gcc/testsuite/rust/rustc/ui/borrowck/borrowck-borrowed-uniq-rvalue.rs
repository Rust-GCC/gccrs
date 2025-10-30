//buggy.rs

#![feature(box_syntax)]

use std::collections::HashMap;

fn main() {
    let tmp: Box<_>;
    let mut buggy_map: HashMap<usize, &usize> = HashMap::new();
    buggy_map.insert(42, &*Box::new(1)); // { dg-error ".E0716." "" { target *-*-* } }

    // but it is ok if we use a temporary
    tmp = box 2;
    buggy_map.insert(43, &*tmp);
}

