// only-x86_64
// ignore-tidy-linelength
// gate-test-intrinsics
// gate-test-platform_intrinsics
// gate-test-abi_vectorcall
// gate-test-abi_thiscall
// gate-test-abi_ptx
// gate-test-abi_x86_interrupt
// gate-test-abi_amdgpu_kernel
// gate-test-abi_efiapi

// Functions
extern "rust-intrinsic" fn f1() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
extern "platform-intrinsic" fn f2() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
extern "vectorcall" fn f3() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "rust-call" fn f4() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "msp430-interrupt" fn f5() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "ptx-kernel" fn f6() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "x86-interrupt" fn f7() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "thiscall" fn f8() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "amdgpu-kernel" fn f9() {} // { dg-error ".E0658." "" { target *-*-* } }
extern "efiapi" fn f10() {} // { dg-error ".E0658." "" { target *-*-* } }

// Methods in trait definition
trait Tr {
    extern "rust-intrinsic" fn m1(); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    extern "platform-intrinsic" fn m2(); // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    extern "vectorcall" fn m3(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "rust-call" fn m4(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "msp430-interrupt" fn m5(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "ptx-kernel" fn m6(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "x86-interrupt" fn m7(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "thiscall" fn m8(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "amdgpu-kernel" fn m9(); // { dg-error ".E0658." "" { target *-*-* } }
    extern "efiapi" fn m10(); // { dg-error ".E0658." "" { target *-*-* } }

    extern "vectorcall" fn dm3() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "rust-call" fn dm4() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "msp430-interrupt" fn dm5() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "ptx-kernel" fn dm6() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "x86-interrupt" fn dm7() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "thiscall" fn dm8() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "amdgpu-kernel" fn dm9() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "efiapi" fn dm10() {} // { dg-error ".E0658." "" { target *-*-* } }
}

struct S;

// Methods in trait impl
impl Tr for S {
    extern "rust-intrinsic" fn m1() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    extern "platform-intrinsic" fn m2() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    extern "vectorcall" fn m3() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "rust-call" fn m4() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "msp430-interrupt" fn m5() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "ptx-kernel" fn m6() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "x86-interrupt" fn m7() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "thiscall" fn m8() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "amdgpu-kernel" fn m9() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "efiapi" fn m10() {} // { dg-error ".E0658." "" { target *-*-* } }
}

// Methods in inherent impl
impl S {
    extern "rust-intrinsic" fn im1() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    extern "platform-intrinsic" fn im2() {} // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    extern "vectorcall" fn im3() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "rust-call" fn im4() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "msp430-interrupt" fn im5() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "ptx-kernel" fn im6() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "x86-interrupt" fn im7() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "thiscall" fn im8() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "amdgpu-kernel" fn im9() {} // { dg-error ".E0658." "" { target *-*-* } }
    extern "efiapi" fn im10() {} // { dg-error ".E0658." "" { target *-*-* } }
}

// Function pointer types
type A1 = extern "rust-intrinsic" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A2 = extern "platform-intrinsic" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A3 = extern "vectorcall" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A4 = extern "rust-call" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A5 = extern "msp430-interrupt" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A6 = extern "ptx-kernel" fn (); // { dg-error ".E0658." "" { target *-*-* } }
type A7 = extern "x86-interrupt" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A8 = extern "thiscall" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A9 = extern "amdgpu-kernel" fn(); // { dg-error ".E0658." "" { target *-*-* } }
type A10 = extern "efiapi" fn(); // { dg-error ".E0658." "" { target *-*-* } }

// Foreign modules
extern "rust-intrinsic" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "platform-intrinsic" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "vectorcall" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "rust-call" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "msp430-interrupt" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "ptx-kernel" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "x86-interrupt" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "thiscall" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "amdgpu-kernel" {} // { dg-error ".E0658." "" { target *-*-* } }
extern "efiapi" {} // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

