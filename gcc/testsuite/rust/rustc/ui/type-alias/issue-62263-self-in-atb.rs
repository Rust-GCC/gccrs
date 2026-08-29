pub trait Trait {
    type A;
}

pub type Alias = dyn Trait<A = Self::A>;
// { dg-error ".E0433." "" { target *-*-* } .-1 }

fn main() {}

