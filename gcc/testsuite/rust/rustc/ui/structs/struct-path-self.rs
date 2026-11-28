struct S;

trait Tr {
    fn f() {
        let s = Self {};
// { dg-error ".E0071." "" { target *-*-* } .-1 }
        let z = Self::<u8> {};
// { dg-error ".E0071." "" { target *-*-* } .-1 }
// { dg-error ".E0109." "" { target *-*-* } .-2 }
        match s {
            Self { .. } => {}
// { dg-error ".E0071." "" { target *-*-* } .-1 }
        }
    }
}

impl Tr for S {
    fn f() {
        let s = Self {}; // OK
        let z = Self::<u8> {}; // { dg-error ".E0109." "" { target *-*-* } }
        match s {
            Self { .. } => {} // OK
        }
    }
}

impl S {
    fn g() {
        let s = Self {}; // OK
        let z = Self::<u8> {}; // { dg-error ".E0109." "" { target *-*-* } }
        match s {
            Self { .. } => {} // OK
        }
    }
}

fn main() {}

