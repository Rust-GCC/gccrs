#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

static B: i32 = A;
static A: i32 = 5;
static mut M: i32 = 7;
static C: i32 = unsafe { M };

fn main() -> i32 {
    if A != 5 || B != 5 || C != 7 {
        return 1;
    }
    0
}
