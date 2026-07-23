#![feature(type_alias_impl_trait)]

fn main() {
    let y = 42;
    let x = wrong_generic(&y);
    let z: i32 = x; // { dg-error ".E0308." "" { target *-*-* } }
}

type WrongGeneric<T> = impl 'static;
// { dg-error ".E0310." "" { target *-*-* } .-1 }
// { dg-error ".E0310." "" { target *-*-* } .-2 }
// { dg-error ".E0310." "" { target *-*-* } .-3 }

fn wrong_generic<T>(t: T) -> WrongGeneric<T> {
    t
}

