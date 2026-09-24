use std::mem;

#[repr(transparent)]
struct Foo(u32);

const TRANSMUTED_U32: u32 = unsafe { mem::transmute(Foo(3)) };

const fn transmute_fn() -> u32 { unsafe { mem::transmute(Foo(3)) } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const fn transmute_fn_intrinsic() -> u32 { unsafe { std::intrinsics::transmute(Foo(3)) } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const fn transmute_fn_core_intrinsic() -> u32 { unsafe { core::intrinsics::transmute(Foo(3)) } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const unsafe fn unsafe_transmute_fn() -> u32 { mem::transmute(Foo(3)) }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const unsafe fn unsafe_transmute_fn_intrinsic() -> u32 { std::intrinsics::transmute(Foo(3)) }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const unsafe fn unsafe_transmute_fn_core_intrinsic() -> u32 { core::intrinsics::transmute(Foo(3)) }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const fn safe_transmute_fn() -> u32 { mem::transmute(Foo(3)) }
// { dg-error ".E0133." "" { target *-*-* } .-1 }
// { dg-error ".E0133." "" { target *-*-* } .-2 }

const fn safe_transmute_fn_intrinsic() -> u32 { std::intrinsics::transmute(Foo(3)) }
// { dg-error ".E0133." "" { target *-*-* } .-1 }
// { dg-error ".E0133." "" { target *-*-* } .-2 }

const fn safe_transmute_fn_core_intrinsic() -> u32 { core::intrinsics::transmute(Foo(3)) }
// { dg-error ".E0133." "" { target *-*-* } .-1 }
// { dg-error ".E0133." "" { target *-*-* } .-2 }

fn main() {}

