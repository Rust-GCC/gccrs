#![feature(no_core, intrinsics, lang_items, never_type)]
#![no_core]

// { dg-output "attempted to leave type `& !` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `& .i32.` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `& dyn .T<Self>.` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `& i32` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `. const dyn .T<Self>.` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `abi:rust fnptr .. -> ..` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `S1` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `S5` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `.i32, & i32.` uninitialized, which is invalid\n" }
// { dg-output "attempted to instantiate uninhabited type `!`\n" }
// { dg-output "attempted to leave type `S2` uninitialized, which is invalid\n" }
// { dg-output "attempted to leave type `bool` uninitialized, which is invalid\n" }

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
    fn assert_uninit_valid<T>();
}

// ---

#[rustc_layout_scalar_valid_range_start(1)]
struct S1(u32);

#[rustc_layout_scalar_valid_range_end(15)]
struct S2(u32); // uninit-invalid

#[repr(simd)]
struct S3(u32, u32, u32, u32);

struct S4(u32, u32);

struct S5(u32, u32, S1);

trait T {}

fn main() -> i32 {
    unsafe {
        assert_uninit_valid::<&!>();            // uninit-invalid
        assert_uninit_valid::<&[i32]>();        // uninit-invalid
        assert_uninit_valid::<&dyn T>();        // uninit-invalid
        assert_uninit_valid::<&i32>();          // uninit-invalid
        assert_uninit_valid::<*const dyn T>();  // uninit-invalid
        assert_uninit_valid::<fn()> ();         // uninit-invalid
        assert_uninit_valid::<S1>();            // uninit-invalid
        assert_uninit_valid::<S5>();            // uninit-invalid
        assert_uninit_valid::<(i32, &i32)>();   // uninit-invalid
        assert_uninit_valid::<!>();             // uninit-invalid (uninhabited)
        assert_uninit_valid::<S2>();            // uninit-invalid
        assert_uninit_valid::<bool>();          // uninit-invalid

        assert_uninit_valid::<*const [i32]>();  // uninit-valid
        assert_uninit_valid::<*const i32>();    // uninit-valid
        assert_uninit_valid::<[!; 0]>();        // uninit-valid
        assert_uninit_valid::<S3>();            // uninit-valid
        assert_uninit_valid::<S4>();            // uninit-valid
    }
    0
}
