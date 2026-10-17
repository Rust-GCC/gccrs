#![feature(no_core)]
#![no_core]

fn local() {
    let x = 1;
    let _ = const { x + 1 }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn param(p: i32) {
    let _ = const { p }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn const_in_closure() {
    let x = 1;
    let _c = || const { x }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn closure_in_const() {
    let x = 1;
    let _ = const { || x }; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn nested_fn() {
    let x = 1;
    let _ = const {
        fn inner() -> i32 {
            x // { dg-error "attempt to use a non-constant value in a constant .E0435." }
        }
    };
}

fn main() {}
