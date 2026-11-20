#![feature(no_core)]
#![no_core]

extern crate stability_check_b_1 as lib;

use lib::{UnstableStruct, StableStruct, UnstableTrait};
use lib::{unstable_macro, stable_macro};

fn test_struct() {
    let us = UnstableStruct {};
    us.unstable_impl_method();
    us.stable_impl_method();

    let _ = StableStruct {};
}

fn test_trait() {
    let _: <UnstableStruct as UnstableTrait>::UnstableAssocType = 1;
    let _: <UnstableStruct as UnstableTrait>::StableAssocType = 2;
    <lib::UnstableStruct as lib::UnstableTrait>::unstable_trait_method();
    <UnstableStruct as UnstableTrait>::stable_trait_method();


    let _: <StableStruct as UnstableTrait>::UnstableAssocType = 3;
    let _: <StableStruct as UnstableTrait>::StableAssocType = 4;
    <lib::StableStruct as lib::UnstableTrait>::unstable_trait_method();
    <StableStruct as UnstableTrait>::stable_trait_method();
}

fn test_macro() {
    let _ = unstable_macro!();
    let _ = stable_macro!();

    let _ = lib::unstable_macro!();
    let _ = lib::stable_macro!();
}

fn main() {
    test_struct();
    test_trait();
    test_macro();
}


// { dg-error "use of unstable library feature .f_macro_u. .E0658." "" { target *-*-* } 31 }
// { dg-error "use of unstable library feature .f_macro_u. .E0658." "" { target *-*-* } 34 }
// { dg-error "use of unstable library feature .f_externlib_u. .E0658." "" { target *-*-* } 4 }
// { dg-error "use of unstable library feature .f_struct_u. .E0658." "" { target *-*-* } 6 }
// { dg-error "use of unstable library feature .f_trait_u. .E0658." "" { target *-*-* } 6 }
// { dg-error "use of unstable library feature .f_macro_u. .E0658." "" { target *-*-* } 7 }
// { dg-error "use of unstable library feature .f_struct_u. .E0658." "" { target *-*-* } 10 }
// { dg-error "use of unstable library feature .f_implmethod_u. .E0658." "" { target *-*-* } 11 }
// { dg-error "use of unstable library feature .f_struct_u. .E0658." "" { target *-*-* } 18 }
// { dg-error "use of unstable library feature .f_assoctype_u. .E0658." "" { target *-*-* } 18 }
// { dg-error "use of unstable library feature .f_struct_u. .E0658." "" { target *-*-* } 19 }
// { dg-error "use of unstable library feature .f_struct_u. .E0658." "" { target *-*-* } 20 }
// { dg-error "use of unstable library feature .f_traitmethod_u. .E0658." "" { target *-*-* } 20 }
// { dg-error "use of unstable library feature .f_struct_u. .E0658." "" { target *-*-* } 21 }
// { dg-error "use of unstable library feature .f_assoctype_u. .E0658." "" { target *-*-* } 24 }
// { dg-error "use of unstable library feature .f_traitmethod_u. .E0658." "" { target *-*-* } 26 }
