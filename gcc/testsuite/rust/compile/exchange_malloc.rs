#![feature(no_core,lang_items)]
#![no_core]

#[lang = "exchange_malloc"]
unsafe fn _allocate(_size: usize, _align: usize) -> *mut u8 {
    0 as *mut u8
}
