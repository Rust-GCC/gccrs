#![feature(no_core)]
#![no_core]

mod foo {
    mod bar {}

    fn baz() -> bar {} // { dg-error "expected type, found module" }
}
