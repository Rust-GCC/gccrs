// { dg-output "d\r*\n" }
// { dg-additional-options "-w" }
#![feature(no_core)]
#![feature(lang_items)]
#![no_core]

extern "C" {
    fn printf(s: *const i8, ...);
}

#[lang = "sized"]
pub trait Sized {}

#[lang = "drop"]
pub trait Drop {
    fn drop(&mut self);
}

struct Droppable;

impl Drop for Droppable {
    fn drop(&mut self) {
        let msg = "d\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

fn f() {
    let _x = Droppable;
}

fn main() -> i32 {
    f();
    0
}
