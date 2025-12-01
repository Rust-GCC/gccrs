#![feature(box_syntax)]

#[allow(non_camel_case_types)]
trait bar { fn dup(&self) -> Self; fn blah<X>(&self); }
impl bar for i32 { fn dup(&self) -> i32 { *self } fn blah<X>(&self) {} }
impl bar for u32 { fn dup(&self) -> u32 { *self } fn blah<X>(&self) {} }

fn main() {
    10.dup::<i32>(); // { dg-error ".E0107." "" { target *-*-* } }
    10.blah::<i32, i32>(); // { dg-error ".E0107." "" { target *-*-* } }
    (box 10 as Box<dyn bar>).dup();
// { dg-error ".E0038." "" { target *-*-* } .-1 }
// { dg-error ".E0038." "" { target *-*-* } .-2 }
}

