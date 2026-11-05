// { dg-additional-options "-frust-crate-type=rlib" }

#![feature(no_core)]
#![no_core]
#![unstable(feature = "f_externlib_u", issue = "none")]
#![feature(staged_api)]

#[unstable(feature = "f_method_u", issue = "none")]
pub fn unstable_method() {}

#[unstable(feature = "f_struct_u", issue = "none")]
pub struct UnstableStruct;

impl UnstableStruct {
    #[unstable(feature = "f_implmethod_u", issue = "none")]
    pub fn unstable_impl_method(&self) {}

    #[stable(feature = "f_implmethod_s", since = "")]
    pub fn stable_impl_method(&self) {}
}

#[stable(feature = "f_struct_s", since = "")]
pub struct StableStruct;

#[unstable(feature = "f_trait_u", issue = "none")]
pub trait UnstableTrait {
    #[unstable(feature = "f_assoctype_u", issue = "none")]
    type UnstableAssocType;

    #[stable(feature = "f_assoctype_s", since = "")]
    type StableAssocType;

    #[unstable(feature = "f_traitmethod_u", issue = "none")]
    fn unstable_trait_method();

    #[stable(feature = "f_traitmethod_s", since = "")]
    fn stable_trait_method();
}

#[unstable(feature = "f_traitimpl_u", issue = "none")]
impl UnstableTrait for UnstableStruct {
    type UnstableAssocType = i32;
    type StableAssocType = i32;
    fn unstable_trait_method() {}
    fn stable_trait_method() {}
}

#[stable(feature = "f_traitimpl_s", since = "")]
impl UnstableTrait for StableStruct {
    type UnstableAssocType = i32;
    type StableAssocType = i32;
    fn unstable_trait_method() {}
    fn stable_trait_method() {}
}

#[unstable(feature = "f_macro_u", issue = "none")]
#[macro_export]
macro_rules! unstable_macro {
    () => {
        12
    };
}

#[stable(feature = "f_macro_s", since = "")]
#[macro_export]
macro_rules! stable_macro {
    () => {
        13
    };
}
