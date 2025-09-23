fn main() { }

fn unconstrained_type() {
    []; // { dg-error ".E0282." "" { target *-*-* } }
}

