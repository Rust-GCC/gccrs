struct S;

trait Tr {
    type A;
}

impl Tr for S {
    type A = S;
}

fn f<T: Tr>() {
    let s = T::A {};
// { dg-error ".E0071." "" { target *-*-* } .-1 }
    let z = T::A::<u8> {};
// { dg-error ".E0071." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }
    match S {
        T::A {} => {}
// { dg-error ".E0071." "" { target *-*-* } .-1 }
    }
}

fn g<T: Tr<A = S>>() {
    let s = T::A {}; // OK
    let z = T::A::<u8> {}; // { dg-error ".E0109." "" { target *-*-* } }
    match S {
        T::A {} => {} // OK
    }
}

fn main() {
    let s = S::A {}; // { dg-error ".E0223." "" { target *-*-* } }
    let z = S::A::<u8> {}; // { dg-error ".E0223." "" { target *-*-* } }
    match S {
        S::A {} => {} // { dg-error ".E0223." "" { target *-*-* } }
    }
}

