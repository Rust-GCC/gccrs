#![crate_type = "lib"]

extern {
    #[ffi_pure] // { dg-error ".E0658." "" { target *-*-* } }
    pub fn foo();
}

