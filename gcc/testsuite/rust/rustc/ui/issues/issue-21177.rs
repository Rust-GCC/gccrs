trait Trait {
    type A;
    type B;
}

fn foo<T: Trait<A = T::B>>() { }
// { dg-error ".E0391." "" { target *-*-* } .-1 }

fn main() { }

