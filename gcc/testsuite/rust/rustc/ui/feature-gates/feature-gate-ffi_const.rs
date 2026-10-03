#![crate_type = "lib"]

extern {
    #[ffi_const] // { dg-error ".E0658." "" { target *-*-* } }
    pub fn foo();
}

