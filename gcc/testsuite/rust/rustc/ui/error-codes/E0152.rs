// normalize-stderr-test "loaded from .*liballoc-.*.rlib" -> "loaded from SYSROOT/liballoc-*.rlib"
#![feature(lang_items)]

#[lang = "owned_box"]
struct Foo; // { dg-error ".E0152." "" { target *-*-* } }

fn main() {
}

