// compile-flags:-C panic=abort

#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(
    info: &'static PanicInfo, // { dg-error "" "" { target *-*-* } }
) -> !
{
    loop {}
}

