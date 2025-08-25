// edition:2018

// Built-in attribute
use inline as imported_inline;
mod builtin {
    pub use inline as imported_inline;
}

// Tool module
use rustfmt as imported_rustfmt;
mod tool_mod {
    pub use rustfmt as imported_rustfmt;
}

#[imported_inline] // { dg-error "" "" { target *-*-* } }
#[builtin::imported_inline] // { dg-error "" "" { target *-*-* } }
#[imported_rustfmt::skip] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
#[tool_mod::imported_rustfmt::skip] // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {}

