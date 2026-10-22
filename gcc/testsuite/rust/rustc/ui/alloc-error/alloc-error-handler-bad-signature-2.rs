// compile-flags:-C panic=abort

#![feature(alloc_error_handler, panic_handler)]
#![no_std]
#![no_main]

struct Layout;

#[alloc_error_handler]
fn oom(
    info: Layout, // { dg-error "" "" { target *-*-* } }
) { // { dg-error "" "" { target *-*-* } }
    loop {}
}

#[panic_handler]
fn panic(_: &core::panic::PanicInfo) -> ! { loop {} }

