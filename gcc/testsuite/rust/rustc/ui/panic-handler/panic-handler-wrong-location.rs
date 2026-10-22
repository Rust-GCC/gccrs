// compile-flags:-C panic=abort

#![no_std]
#![no_main]

#[panic_handler] // { dg-error ".E0718." "" { target *-*-* } }
#[no_mangle]
static X: u32 = 42;

