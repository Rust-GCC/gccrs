// ignore-wasm32-bare compiled with panic=abort by default
// compile-flags: -C no-prepopulate-passes -Cpasses=name-anon-globals

#![crate_type = "lib"]

extern {
// CHECK: Function Attrs: nounwind
// CHECK-NEXT: declare void @extern_fn
    fn extern_fn();
// CHECK-NOT: Function Attrs: nounwind
// CHECK: declare void @unwinding_extern_fn
    #[unwind(allowed)] // { dg-error ".E0658." "" { target *-*-* } }
    fn unwinding_extern_fn();
}

pub unsafe fn force_declare() {
    extern_fn();
    unwinding_extern_fn();
}

