// build-fail
// compile-flags:-Zpolymorphize=on
#![feature(const_generics, rustc_attrs)]
// { dg-warning "" "" { target *-*-* } .-1 }

// This test checks that the polymorphization analysis correctly detects unused const
// parameters in functions.

// Function doesn't have any generic parameters to be unused.
#[rustc_polymorphize_error]
pub fn no_parameters() {}

// Function has an unused generic parameter.
#[rustc_polymorphize_error]
pub fn unused<const T: usize>() {
// { dg-error "" "" { target *-*-* } .-1 }
}

// Function uses generic parameter in value of a binding.
#[rustc_polymorphize_error]
pub fn used_binding<const T: usize>() -> usize {
    let x: usize = T;
    x
}

// Function uses generic parameter in substitutions to another function.
#[rustc_polymorphize_error]
pub fn used_substs<const T: usize>() {
    unused::<T>()
}

fn main() {
    no_parameters();
    unused::<1>();
    used_binding::<1>();
    used_substs::<1>();
}

