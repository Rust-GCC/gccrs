pub trait Tr<'a> {
    type Out;
}

pub fn f<'a, T: Tr<'a>>() -> <T as Tr<'a>>::Out {}
// { dg-error ".E0308." "" { target *-*-* } .-1 }

pub fn main() {}

