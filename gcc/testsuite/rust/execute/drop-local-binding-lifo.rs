// { dg-output "C\r*\nB\r*\nA\r*\n" }
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

struct A;
struct B;
struct C;

impl Drop for A {
    fn drop(&mut self) {
        let msg = "A\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

impl Drop for B {
    fn drop(&mut self) {
        let msg = "B\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

impl Drop for C {
    fn drop(&mut self) {
        let msg = "C\n\0" as *const str as *const i8;
        unsafe {
            printf(msg);
        }
    }
}

fn main() -> i32 {
    let _a = A;
    let _b = B;
    let _c = C;

    0
}