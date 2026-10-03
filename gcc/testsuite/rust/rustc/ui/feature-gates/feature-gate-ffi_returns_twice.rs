#![crate_type = "lib"]

extern {
    #[ffi_returns_twice] // { dg-error ".E0658." "" { target *-*-* } }
    pub fn foo();
}

