use std::mem;

fn main() {
    mem::transmute(0);
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

