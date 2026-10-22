#![feature(main)]

#[main]
fn bar() {
}

#[main]
fn foo() { // { dg-error ".E0137." "" { target *-*-* } }
}

