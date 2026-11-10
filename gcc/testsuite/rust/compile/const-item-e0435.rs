#![feature(no_core)]
#![no_core]

fn local() {
    let x = 1;
    const Y: i32 = x; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn param(p: i32) {
    const Y: i32 = p; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn in_closure() {
    let x = 1;
    let _c = || {
        const Y: i32 = x; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
    };
}

fn in_static() {
    let x = 1;
    static Z: i32 = x; // { dg-error "attempt to use a non-constant value in a constant .E0435." }
}

fn main() {}
