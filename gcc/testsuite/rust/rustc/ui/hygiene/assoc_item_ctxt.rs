// ignore-pretty pretty-printing is unhygienic

#![feature(decl_macro)]
#![allow(unused)]

mod ok {
    macro mac_trait_item($method: ident) {
        fn $method();
    }

    trait Tr {
        mac_trait_item!(method);
    }

    macro mac_trait_impl() {
        impl Tr for u8 { // OK
            fn method() {} // OK
        }
    }

    mac_trait_impl!();
}

mod error {
    macro mac_trait_item() {
        fn method();
    }

    trait Tr {
        mac_trait_item!();
    }

    macro mac_trait_impl() {
        impl Tr for u8 { // { dg-error ".E0046." "" { target *-*-* } }
            fn method() {} // { dg-error ".E0407." "" { target *-*-* } }
        }
    }

    mac_trait_impl!();
}

fn main() {}

