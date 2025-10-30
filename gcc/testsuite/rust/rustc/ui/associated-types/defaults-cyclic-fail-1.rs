#![feature(associated_type_defaults)]

// Having a cycle in assoc. type defaults is okay...
trait Tr {
    type A = Self::B;
    type B = Self::A;
}

impl Tr for () {}

impl Tr for u8 {
    type A = u8;
}

impl Tr for u16 {
    type B = ();
}

impl Tr for u32 {
    type A = ();
    type B = u8;
}

// ...but not in an impl that redefines one of the types.
impl Tr for bool {
    type A = Box<Self::B>;
// { dg-error ".E0275." "" { target *-*-* } .-1 }
}
// (the error is shown twice for some reason)

impl Tr for usize {
    type B = &'static Self::A;
// { dg-error ".E0275." "" { target *-*-* } .-1 }
}

fn main() {
    // We don't check that the types project correctly because the cycle errors stop compilation
    // before `main` is type-checked.
    // `defaults-cyclic-pass-1.rs` does this.
}

