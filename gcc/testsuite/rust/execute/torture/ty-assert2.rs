#![feature(no_core, intrinsics, lang_items, never_type)]
#![no_core]

// { dg-output "attempted to zero-initialize type `& !`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `& .i32.`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `& dyn .T<Self>.`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `& i32`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `. const dyn .T<Self>.`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `abi:rust fnptr .. -> ..`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `S1`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `S5`, which is invalid\n" }
// { dg-output "attempted to zero-initialize type `.i32, & i32.`, which is invalid\n" }
// { dg-output "attempted to instantiate uninhabited type `!`\n" }

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
    fn assert_zero_valid<T>();
}

// ---

#[rustc_layout_scalar_valid_range_start(1)]
struct S1(u32);

#[rustc_layout_scalar_valid_range_end(15)]
struct S2(u32);

#[repr(simd)]
struct S3(u32, u32, u32, u32);

struct S4(u32, u32);

struct S5(u32, u32, S1);

trait T {}

fn main() -> i32 {
    unsafe {
        assert_zero_valid::<&!>();            // zero-invalid
        assert_zero_valid::<&[i32]>();        // zero-invalid
        assert_zero_valid::<&dyn T>();        // zero-invalid
        assert_zero_valid::<&i32>();          // zero-invalid
        assert_zero_valid::<*const dyn T>();  // zero-invalid
        assert_zero_valid::<fn()> ();         // zero-invalid
        assert_zero_valid::<S1>();            // zero-invalid
        assert_zero_valid::<S5>();            // zero-invalid
        assert_zero_valid::<(i32, &i32)>();   // zero-invalid
        assert_zero_valid::<!>();             // zero-invalid (uninhabited)

        assert_zero_valid::<*const [i32]>();  // zero-valid
        assert_zero_valid::<*const i32>();    // zero-valid
        assert_zero_valid::<[!; 0]>();        // zero-valid
        assert_zero_valid::<S2>();            // zero-valid
        assert_zero_valid::<S3>();            // zero-valid
        assert_zero_valid::<S4>();            // zero-valid
        assert_zero_valid::<bool>();          // zero-valid
    }
    0
}
