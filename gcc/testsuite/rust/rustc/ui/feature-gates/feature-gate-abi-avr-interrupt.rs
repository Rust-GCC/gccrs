// Test that the AVR interrupt ABI cannot be used when avr_interrupt
// feature gate is not used.

extern "avr-interrupt" fn foo() {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {
    foo();
}

