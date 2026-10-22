// `#![derive]` raises errors when it occurs at contexts other than ADT
// definitions.

#[derive(Debug)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
mod derive {
    mod inner { #![derive(Debug)] }
// { dg-error ".E0774." "" { target *-*-* } .-1 }

    #[derive(Debug)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
    fn derive() { }

    #[derive(Copy, Clone)] // (can't derive Debug for unions)
    union U { f: i32 }

    #[derive(Debug)]
    struct S;

    #[derive(Debug)]
    enum E { }

    #[derive(Debug)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
    type T = S;

    #[derive(Debug)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
    impl S { }
}

fn main() {}

