#![feature(no_core)]
#![no_core]

mod a {
    trait A {
        fn foo();
    }

    struct B;

    impl A for B {
        fn foo() {}
    }
}

use a::*;

pub fn bar() {
    foo() // { dg-error "Cannot find path" }
}
