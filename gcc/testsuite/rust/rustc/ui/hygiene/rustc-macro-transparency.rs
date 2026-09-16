#![feature(decl_macro, rustc_attrs)]

#[rustc_macro_transparency = "transparent"]
macro transparent() {
    struct Transparent;
    let transparent = 0;
}
#[rustc_macro_transparency = "semitransparent"]
macro semitransparent() {
    struct SemiTransparent;
    let semitransparent = 0;
}
#[rustc_macro_transparency = "opaque"]
macro opaque() {
    struct Opaque;
    let opaque = 0;
}

fn main() {
    transparent!();
    semitransparent!();
    opaque!();

    Transparent; // OK
    SemiTransparent; // OK
    Opaque; // { dg-error ".E0425." "" { target *-*-* } }

    transparent; // OK
    semitransparent; // { dg-error ".E0423." "" { target *-*-* } }
    opaque; // { dg-error ".E0423." "" { target *-*-* } }
}

