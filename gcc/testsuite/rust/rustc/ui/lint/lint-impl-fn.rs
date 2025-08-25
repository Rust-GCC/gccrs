#![allow(while_true)]
#![allow(dead_code)]

struct A(isize);

impl A {
    fn foo(&self) { while true {} }

    #[deny(while_true)]
    fn bar(&self) { while true {} } // { dg-error "" "" { target *-*-* } }
}

#[deny(while_true)]
mod foo {
    struct B(isize);

    impl B {
        fn foo(&self) { while true {} } // { dg-error "" "" { target *-*-* } }

        #[allow(while_true)]
        fn bar(&self) { while true {} }
    }
}

#[deny(while_true)]
fn main() {
    while true {} // { dg-error "" "" { target *-*-* } }
}

#[deny(while_true)]
fn bar() {
    while cfg!(unix) {} // no error
}

