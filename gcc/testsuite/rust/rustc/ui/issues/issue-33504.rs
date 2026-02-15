// Shadowing a unit-like enum in a closure

struct Test;

fn main() {
    || {
        let Test = 1; // { dg-error ".E0308." "" { target *-*-* } }
    };
}

