#![feature(rustc_attrs)]
#![feature(no_core)]
#![no_core]
#![feature(extended_key_value_attributes)]
#![doc = concat!("AB")]

#[rustc_builtin_macro]
macro_rules! concat {
    () => {{}};
}
