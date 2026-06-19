// compile-flags:-C panic=abort

#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(
    info: PanicInfo, // { dg-error "" "" { target *-*-* } }
) -> () // { dg-error "" "" { target *-*-* } }
{
}

