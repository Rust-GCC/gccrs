// aux-build:macro_crate_nonterminal.rs

mod foo {
    #[macro_use]
    extern crate macro_crate_nonterminal;  // { dg-error ".E0468." "" { target *-*-* } }
}

fn main() {
}

