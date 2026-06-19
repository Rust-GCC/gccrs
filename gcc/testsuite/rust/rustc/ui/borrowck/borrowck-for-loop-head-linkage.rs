use std::iter::repeat;

fn main() {
    let mut vector = vec![1, 2];
    for &x in &vector {
        let cap = vector.capacity();
        vector.extend(repeat(0));      // { dg-error ".E0502." "" { target *-*-* } }
        vector[1] = 5;   // { dg-error ".E0502." "" { target *-*-* } }
    }
}

