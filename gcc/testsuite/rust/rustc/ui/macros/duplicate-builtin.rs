// compile-flags:--crate-type lib
#![feature(decl_macro)]
#![feature(rustc_attrs)]

#[rustc_builtin_macro]
pub macro test($item:item) {
// { dg-note "" "" { target *-*-* } .-1 }
    /* compiler built-in */
}

mod inner {
    #[rustc_builtin_macro]
    pub macro test($item:item) {
// { dg-error ".E0773." "" { target *-*-* } .-1 }
        /* compiler built-in */
    }
}

