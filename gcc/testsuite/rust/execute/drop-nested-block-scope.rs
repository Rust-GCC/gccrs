// { dg-output "inner\r*\nmiddle2\r*\nmiddle\r*\nouter2\r*\nouter\r*\n" }
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

struct Outer;
struct Outer2;
struct Middle;
struct Middle2;
struct Inner;

impl Drop for Outer {
    fn drop(&mut self) {
        let msg = "outer\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

impl Drop for Outer2 {
    fn drop(&mut self) {
        let msg = "outer2\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

impl Drop for Middle {
    fn drop(&mut self) {
        let msg = "middle\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

impl Drop for Middle2 {
    fn drop(&mut self) {
        let msg = "middle2\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

impl Drop for Inner {
    fn drop(&mut self) {
        let msg = "inner\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

fn main() -> i32 {
    let _outer = Outer;

    {
        let _middle = Middle;

        {
            let _inner = Inner;
        }

        let _middle2 = Middle2;
    }

    let _outer2 = Outer2;

    0
}
