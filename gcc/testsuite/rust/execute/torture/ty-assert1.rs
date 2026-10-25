#![feature(no_core, intrinsics, lang_items, never_type)]
#![no_core]

// { dg-output "attempted to instantiate uninhabited type `!`\n" }
// { dg-output "attempted to instantiate uninhabited type `E1`\n" }
// { dg-output "attempted to instantiate uninhabited type `.!; 1.`\n" }
// { dg-output "attempted to instantiate uninhabited type `.i32, !.`\n" }
// { dg-output "attempted to instantiate uninhabited type `E2`\n" }
// { dg-output "attempted to instantiate uninhabited type `S1`\n" }
// { dg-output "attempted to instantiate uninhabited type `S2`\n" }
// { dg-output "attempted to instantiate uninhabited type `S3`\n" }

#[lang= "sized"]
pub trait Sized {}

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
}

enum E1 {}

enum E2 {
    V1(!),
    V2(E1),
}

struct S1(!);

struct S2(i32, S1);

struct S3{
    _x: E1,
}

enum E3 {
    V1(!),
    V2(E2),
    V3(S1),
    V4(S2),
    V5(i32),
}

fn main() -> i32 {
    unsafe {
        assert_inhabited::<!>();        // uninhabited
        assert_inhabited::<E1>();       // uninhabited
        assert_inhabited::<[!; 1]>();   // uninhabited
        assert_inhabited::<(i32, !)>(); // uninhabited
        assert_inhabited::<E2>();       // uninhabited
        assert_inhabited::<S1>();       // uninhabited
        assert_inhabited::<S2>();       // uninhabited
        assert_inhabited::<S3>();       // uninhabited

        assert_inhabited::<E3>();       // inhabited
        assert_inhabited::<[!; 0]>();   // inhabited
        assert_inhabited::<&!>();       // inhabited
        assert_inhabited::<*const !>(); // inhabited
        assert_inhabited::<fn() -> !>();// inhabited
        assert_inhabited::<()>();       // inhabited
    }
    0
}
