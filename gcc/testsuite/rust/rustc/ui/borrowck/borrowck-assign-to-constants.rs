static foo: isize = 5;

fn main() {
    // assigning to various global constants
    foo = 6; // { dg-error ".E0594." "" { target *-*-* } }
}

