#![feature(no_core)]
#![no_core]

mod foo {
    mod bar {}

    struct bar; // { dg-error "defined multiple times" }
}
