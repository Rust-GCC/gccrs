fn main() {
    // N.B., this (almost) typechecks when default binding modes are enabled.
    for (ref i,) in [].iter() {
        i.clone();
// { dg-error ".E0282." "" { target *-*-* } .-1 }
    }
}

