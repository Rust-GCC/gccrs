trait Trait<'a> {
    type A;
    type B;
}

fn foo<'a, T: Trait<'a>>(value: T::A) {
    let new: T::B = unsafe { std::mem::transmute(value) };
// { dg-error ".E0512." "" { target *-*-* } .-1 }
}

fn main() { }

