#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

static mut S: i32 = 4;
static T: i32 = 5;

const X: i32 = unsafe { S }; // { dg-error "constants cannot refer to statics .E0013." }
const Y: i32 = T; // { dg-error "constants cannot refer to statics .E0013." }

// statics may read other statics
static U: i32 = T;

pub const fn f() -> i32 {
    T // { dg-error "constant functions cannot refer to statics .E0013." }
}
