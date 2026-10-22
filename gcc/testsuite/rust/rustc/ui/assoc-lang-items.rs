#![feature(lang_items)]

trait Foo {
    #[lang = "dummy_lang_item_1"] // { dg-error ".E0522." "" { target *-*-* } }
    fn foo() {}

    #[lang = "dummy_lang_item_2"] // { dg-error ".E0522." "" { target *-*-* } }
    fn bar();

    #[lang = "dummy_lang_item_3"] // { dg-error ".E0522." "" { target *-*-* } }
    type MyType;
}

struct Bar;

impl Bar {
    #[lang = "dummy_lang_item_4"] // { dg-error ".E0522." "" { target *-*-* } }
    fn test() {}
}

fn main() {}

