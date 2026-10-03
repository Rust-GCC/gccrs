// Test that the MSP430 interrupt ABI cannot be used when msp430_interrupt
// feature gate is not used.

// ignore-riscv64 msp430 is not supported

extern "msp430-interrupt" fn foo() {}
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {
    foo();
}

