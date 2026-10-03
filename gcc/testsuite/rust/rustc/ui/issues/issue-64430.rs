// compile-flags:-C panic=abort

#![no_std]
pub struct Foo;

fn main() {
    Foo.bar()
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop{}
}

