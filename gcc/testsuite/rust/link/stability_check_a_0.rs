#![feature(no_core)]
#![no_core]

#![feature(f_externlib_u)]
#![feature(f_trait_u)]
#![feature(f_struct_u)]
#![feature(f_implmethod_u)]
#![feature(f_assoctype_u)]
#![feature(f_traitmethod_u)]
#![feature(f_macro_u)]

extern crate stability_check_a_1 as lib;

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
