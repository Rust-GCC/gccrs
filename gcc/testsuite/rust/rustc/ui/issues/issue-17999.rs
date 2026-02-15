#![deny(unused_variables)]

fn main() {
    for _ in 1..101 {
        let x = (); // { dg-error "" "" { target *-*-* } }
        match () {
            a => {} // { dg-error "" "" { target *-*-* } }
        }
    }
}

