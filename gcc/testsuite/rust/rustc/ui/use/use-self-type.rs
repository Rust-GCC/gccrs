struct S;

impl S {
    fn f() {}
    fn g() {
        use Self::f; // { dg-error ".E0432." "" { target *-*-* } }
        pub(in Self::f) struct Z; // { dg-error ".E0433." "" { target *-*-* } }
    }
}

fn main() {}

