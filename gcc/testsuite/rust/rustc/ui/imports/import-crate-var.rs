// aux-build:import_crate_var.rs

#[macro_use] extern crate import_crate_var;

fn main() {
    m!();
// { dg-error "" "" { target *-*-* } .-1 }
}

