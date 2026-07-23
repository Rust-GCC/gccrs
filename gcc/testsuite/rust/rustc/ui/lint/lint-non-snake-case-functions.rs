#![deny(non_snake_case)]
#![allow(dead_code)]

struct Foo;

impl Foo {
    fn Foo_Method() {}
// { dg-error "" "" { target *-*-* } .-1 }

    // Don't allow two underscores in a row
    fn foo__method(&self) {}
// { dg-error "" "" { target *-*-* } .-1 }

    pub fn xyZ(&mut self) {}
// { dg-error "" "" { target *-*-* } .-1 }

    fn render_HTML() {}
// { dg-error "" "" { target *-*-* } .-1 }
}

trait X {
    fn ABC();
// { dg-error "" "" { target *-*-* } .-1 }

    fn a_b_C(&self) {}
// { dg-error "" "" { target *-*-* } .-1 }

    fn something__else(&mut self);
// { dg-error "" "" { target *-*-* } .-1 }
}

impl X for Foo {
    // These errors should be caught at the trait definition not the impl
    fn ABC() {}
    fn something__else(&mut self) {}
}

fn Cookie() {}
// { dg-error "" "" { target *-*-* } .-1 }

pub fn bi_S_Cuit() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() { }

