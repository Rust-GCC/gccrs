// { dg-options "-fno-exceptions -w" }
/* { dg-output "init_two\r*\n1\r*\n2\r*\ninit_two\r*\n3\r*\n4\r*\ndump_from_two\r*\n3\r*\n4\r*\ndump_from_two\r*\n1\r*\n2\r*\n" } */
#![feature(no_core)]
#![no_core]
#![feature(lang_items)]
#[lang = "sized"]
pub trait Sized {}
#[lang = "copy"]
pub trait Copy {}
#[lang = "drop"]
pub trait Drop {
    fn drop(&mut self);
}
extern "C" {
    fn puts(s: *const i8);
}
extern "C" {
    fn printf(s: *const i8, ...);
}
fn dump(message: &str) {
    unsafe {
        let b = message as *const str;
        let c = b as *const i8;
        puts(c);
    }
}
fn dump_number(num: i32) {
    unsafe {
        let a = "%i\n\0";
        let b = a as *const str;
        let c = b as *const i8;
        printf(c, num);
    }
}
struct Two {
    a: i32,
    b: i32,
}
impl Two {
    fn new(a : i32, b : i32) -> Two {
        dump("init_two");
        dump_number(a);
        dump_number(b);
        Two { a: a, b: b }
    }
}
impl Drop for Two {
    fn drop(&mut self) {
        dump("dump_from_two");
        dump_number(self.a);
        dump_number(self.b);
    }
}
fn main() -> i32 {
    let mut a : Two;
    let mut b : Two;
    a = Two::new(1,2);
    b = Two::new(3,4);
    b = a;
    0
}
