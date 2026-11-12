#![feature(no_core)]
#![feature(intrinsics)]
#![feature(lang_items)]
#![no_core]

// { dg-output "attempted to leave type `& !` uninitialized, which is invalid\r*\nattempted to leave type `Foo` uninitialized, which is invalid\r*\n" }

#[lang = "sized"]
trait Sized {}

#[lang = "panic"]
pub fn panic(s: &'static str) -> ! {
    unsafe {
        puts(s as *const str as *const i8);
    }
}

extern "C" {
    fn puts(s: *const i8);
}

extern "rust-intrinsic" {
    fn assert_inhabited<T>();
    fn assert_zero_valid<T>();
    fn assert_uninit_valid<T>();
}

#[rustc_layout_scalar_valid_range_start(1)]
struct Foo(u32);

fn foo() {
    unsafe { assert_uninit_valid::<&!>() };
    unsafe { assert_uninit_valid::<Foo>() };
    unsafe { assert_uninit_valid::<i32>() };
}

fn main() -> i32 {
    foo();

    0
}
