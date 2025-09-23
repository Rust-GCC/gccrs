// ignore-tidy-linelength

// aux-build:two_macros.rs
// compile-flags:--extern non_existent

mod n {
    extern crate two_macros;
}

mod m {
    fn check() {
        two_macros::m!(); // { dg-error ".E0433." "" { target *-*-* } }
    }
}

macro_rules! define_std_as_non_existent {
    () => {
        extern crate std as non_existent;
// { dg-error "" "" { target *-*-* } .-1 }
    }
}
define_std_as_non_existent!();

fn main() {}

