// Generic type aliases used to ICE in the reachability pass because their
// generic parameters were never resolved into the type context.
#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

pub type Alias<T> = T;
