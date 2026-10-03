// Regression test; used to ICE with 'visit_mac_call disabled by default' due to a
// `MutVisitor` in `fn make_all_value_bindings_mutable` (`parse/parser/pat.rs`).

macro_rules! mac1 {
    ($eval:expr) => {
        let mut $eval = ();
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

macro_rules! mac2 {
    ($eval:pat) => {
        let mut $eval = ();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    };
}

fn foo() {
    mac1! { does_not_exist!() }
// { dg-error "" "" { target *-*-* } .-1 }
    mac2! { does_not_exist!() }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

