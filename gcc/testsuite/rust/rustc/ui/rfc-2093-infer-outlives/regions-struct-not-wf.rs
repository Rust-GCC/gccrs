// Various examples of structs whose fields are not well-formed.

#![allow(dead_code)]

trait Trait<'a, T> {
    type Out;
}
trait Trait1<'a, 'b, T> {
    type Out;
}

impl<'a, T> Trait<'a, T> for usize {
    type Out = &'a T; // { dg-error ".E0309." "" { target *-*-* } }
}

struct RefOk<'a, T:'a> {
    field: &'a T
}

impl<'a, T> Trait<'a, T> for u32 {
    type Out = RefOk<'a, T>; // { dg-error ".E0309." "" { target *-*-* } }
}

impl<'a, 'b, T> Trait1<'a, 'b, T> for u32 {
    type Out = &'a &'b T; // { dg-error ".E0491." "" { target *-*-* } }
}

fn main() { }

