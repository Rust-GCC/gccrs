// compile-flags:-C panic=abort

#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic<T>(pi: &PanicInfo) -> ! {
// { dg-error "" "" { target *-*-* } .-1 }
    loop {}
}

