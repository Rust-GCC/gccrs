// compile-flags:-C panic=abort

#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic() -> ! { // { dg-error "" "" { target *-*-* } }
    loop {}
}

