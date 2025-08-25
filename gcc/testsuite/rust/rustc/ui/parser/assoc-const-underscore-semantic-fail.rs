// Semantically, an associated constant cannot use `_` as a name.

fn main() {}

const _: () = {
    pub trait A {
        const _: () = (); // { dg-error "" "" { target *-*-* } }
    }
    impl A for () {
        const _: () = (); // { dg-error ".E0438." "" { target *-*-* } }
// { dg-error ".E0438." "" { target *-*-* } .-1 }
    }
    struct B;
    impl B {
        const _: () = (); // { dg-error "" "" { target *-*-* } }
    }
};

