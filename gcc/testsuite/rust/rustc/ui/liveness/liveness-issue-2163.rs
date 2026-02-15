use std::vec::Vec;

fn main() {
    let a: Vec<isize> = Vec::new();
    a.iter().all(|_| -> bool {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    });
}

