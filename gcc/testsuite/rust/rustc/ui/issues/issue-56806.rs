pub trait Trait {
    fn dyn_instead_of_self(self: Box<dyn Trait>);
// { dg-error ".E0307." "" { target *-*-* } .-1 }
}

pub fn main() {}

