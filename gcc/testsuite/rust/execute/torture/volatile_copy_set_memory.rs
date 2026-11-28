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
    pub fn volatile_copy_nonoverlapping_memory<T>(dst: *mut T, src: *const T, count: usize);
    pub fn volatile_copy_memory<T>(dst: *mut T, src: *const T, count: usize);
    pub fn volatile_set_memory<T>(dst: *mut T, val: u8, count: usize);
}

fn main() -> i32 {
    unsafe {
        let mut value: [u8; 8] = [0x67u8; 8];
        let mut ptr = &mut value as *mut [u8] as *mut u8;

        volatile_set_memory(ptr, 0x02, 5);
        if value[0] != 0x02 || value[5] != 0x67 {
            return 1;
        }
    }

    unsafe {
        let mut value: [u32; 8] = [0x67; 8];
        let mut ptr = &mut value as *mut [u32] as *mut u32;
        let mut value2: [u32; 8] = [0x10, 0x21, 0x32, 0x43, 0x54, 0xBB, 0x76, 0xAA];
        let ptr2 = &mut value2 as *mut [u32] as *const u32;
        volatile_copy_nonoverlapping_memory(ptr, ptr2, 5);
        if value[0] != 0x10 || value[1] != 0x21 || value[2] != 0x32 {
            return 1;
        }
        if value[3] != 0x43 || value[4] != 0x54 || value[5] != 0x67 {
            return 1;
        }

        let ptr_offset = (ptr as *const u32).add(2);
        volatile_copy_memory(ptr, ptr_offset, 4);
        if value[0] != 0x32 || value[1] != 0x43 || value[2] != 0x54 || value[3] != 0x67 || value[4] != 0x54
        {
            return 1;
        }
    }

    0
}