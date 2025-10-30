// compile-flags:-C panic=abort

#![no_std]
#![no_main]

use core::alloc::Layout;

#[alloc_error_handler] // { dg-error ".E0658." "" { target *-*-* } }
fn oom(info: Layout) -> ! {
    loop {}
}

#[panic_handler]
fn panic(_: &core::panic::PanicInfo) -> ! { loop {} }

