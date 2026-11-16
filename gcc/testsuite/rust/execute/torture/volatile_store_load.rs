#![feature(no_core, lang_items, staged_api, intrinsics)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

// below's helper code copied from issue-1232.rs
extern "rust-intrinsic" {
    #[rustc_const_stable(feature = "const_ptr_offset", since = "1.61.0")]
    fn offset<T>(dst: *const T, offset: isize) -> *const T;
}

#[lang = "const_ptr"]
impl<T> *const T {
    pub const unsafe fn offset(self, count: isize) -> *const T {
        unsafe { offset(self, count) }
    }

    pub const unsafe fn add(self, count: usize) -> Self {
        unsafe { self.offset(count as isize) }
    }

    pub const fn as_ptr(self) -> *const T {
        self as *const T
    }
}

extern "rust-intrinsic" {
    fn volatile_load<T>(src: *const T) -> T;
    fn volatile_store<T>(dst: *mut T, val: T);
    fn unaligned_volatile_load<T>(src: *const T) -> T;
    fn unaligned_volatile_store<T>(dst: *mut T, val: T);
}

fn main() -> i32 {
    let mut value: u32 = 0;
    unsafe {
        // aligned store/load tests
        volatile_store (&mut value as *mut u32, 0x67FACADEu32);
        if volatile_load(&value as *const u32) != 0x67FACADEu32 {
            return 1;
        }

        // unaligned store/load tests
        let value2: [u8; 8] = [0xABu8; 8];
        let mut ptr = &value2 as *const [u8] as *const u8;
        let unaligned_ptr = ptr.add(1);
        let unaligned_u32_ptr = unaligned_ptr as *mut u32;
        unaligned_volatile_store(unaligned_u32_ptr, 0xDEADBEEFu32);

        if unaligned_volatile_load(unaligned_u32_ptr as *const u32) != 0xDEADBEEFu32 {
            return 1;
        }

        if value2[0] != 0xABu8 || value2[5] != 0xABu8 {
            return 1;
        }
    }

    0
}