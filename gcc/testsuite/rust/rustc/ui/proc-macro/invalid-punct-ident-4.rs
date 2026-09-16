// aux-build:invalid-punct-ident.rs

#[macro_use]
extern crate invalid_punct_ident;

lexer_failure!();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

