#![feature(no_core, intrinsics, lang_items, never_type)]
#![no_core]

// dg-output { "attempted to zero-initialize type `SingleVariantDataNonZero`, which is invalid\n" }
// dg-output { "attempted to zero-initialize type `MultiVariantCustomDisc`, which is invalid\n" }
// dg-output { "attempted to zero-initialize type `MultiVariantNonZeroDisc`, which is invalid\n" }
// dg-output { "attempted to zero-initialize type `NonZeroU32`, which is invalid\n" }
// dg-output { "attempted to leave type `SingleVariantDataNonZero` uninitialized, which is invalid\n" }
// dg-output { "attempted to leave type `MultiVariantDefault` uninitialized, which is invalid\n" }
// dg-output { "attempted to leave type `MultiVariantCustomDisc` uninitialized, which is invalid\n" }
// dg-output { "attempted to leave type `MultiVariantCustomDiscZero` uninitialized, which is invalid\n" }
// dg-output { "attempted to leave type `MultiVariantNonZeroDisc` uninitialized, which is invalid\n" }
// dg-output { "attempted to leave type `NonZeroU32` uninitialized, which is invalid\n" }

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
    fn assert_uninit_valid<T>();
}

// ---

#[rustc_layout_scalar_valid_range_start(1)]
struct NonZeroU32(u32);

enum SingleVariantDatalessDefault {
    A,
}

enum SingleVariantDatalessCustomDisc {
    A = 12,
}

enum SingleVariantDataDefault {
    A(u32),
}

enum SingleVariantDataNonZero {
    A(NonZeroU32),
}

enum MultiVariantDefault {
    A(NonZeroU32),
    B(u32),
}

enum MultiVariantCustomDisc {
    A = 12,
    B,
}

enum MultiVariantCustomDiscZero {
    A = 12,
    B = 0,
}

enum MultiVariantNonZeroDisc {
    A = 12,
    B = 13,
}

struct ZST;

fn enum_zero() {
    unsafe {
        assert_zero_valid::<SingleVariantDatalessDefault>();      // zero-valid
        assert_zero_valid::<SingleVariantDatalessCustomDisc>();   // zero-valid
        assert_zero_valid::<SingleVariantDataDefault>();          // zero-valid
        assert_zero_valid::<SingleVariantDataNonZero>();          // zero-invalid

        assert_zero_valid::<MultiVariantDefault>();               // zero-valid
        assert_zero_valid::<MultiVariantCustomDisc>();            // zero-invalid
        assert_zero_valid::<MultiVariantCustomDiscZero>();        // zero-valid
        assert_zero_valid::<MultiVariantNonZeroDisc>();           // zero-invalid

        assert_zero_valid::<NonZeroU32>();                        // zero-invalid
    }
}

fn enum_uninit() {
    unsafe {
        assert_uninit_valid::<SingleVariantDatalessDefault>();    // uninit-valid
        assert_uninit_valid::<SingleVariantDatalessCustomDisc>(); // uninit-valid
        assert_uninit_valid::<SingleVariantDataDefault>();        // uninit-valid
        assert_uninit_valid::<SingleVariantDataNonZero>();        // uninit-invalid

        assert_uninit_valid::<MultiVariantDefault>();             // uninit-invalid
        assert_uninit_valid::<MultiVariantCustomDisc>();          // uninit-invalid
        assert_uninit_valid::<MultiVariantCustomDiscZero>();      // uninit-invalid
        assert_uninit_valid::<MultiVariantNonZeroDisc>();         // uninit-invalid

        assert_uninit_valid::<NonZeroU32>();                      // uninit-invalid
    }
}

fn zst() {
    unsafe {
        assert_zero_valid::<ZST>();                               // zero-valid
        assert_uninit_valid::<ZST>();                             // uninit-valid
    }
}

fn main() -> i32 {
    enum_zero();
    enum_uninit();
    zst();

    0
}
