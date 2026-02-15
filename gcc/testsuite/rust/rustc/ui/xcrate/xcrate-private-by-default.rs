// aux-build:static_priv_by_default.rs

extern crate static_priv_by_default;

fn foo<T>() {}

fn main() {
    // Actual public items should be public
    static_priv_by_default::a;
    static_priv_by_default::b;
    static_priv_by_default::c;
    foo::<static_priv_by_default::d>();
    foo::<static_priv_by_default::e>();

    // publicly re-exported items should be available
    static_priv_by_default::bar::e;
    static_priv_by_default::bar::f;
    static_priv_by_default::bar::g;
    foo::<static_priv_by_default::bar::h>();
    foo::<static_priv_by_default::bar::i>();

    // private items at the top should be inaccessible
    static_priv_by_default::j;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    static_priv_by_default::k;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    static_priv_by_default::l;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    foo::<static_priv_by_default::m>();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    foo::<static_priv_by_default::n>();
// { dg-error ".E0603." "" { target *-*-* } .-1 }

    // public items in a private mod should be inaccessible
    static_priv_by_default::foo::a;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    static_priv_by_default::foo::b;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    static_priv_by_default::foo::c;
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    foo::<static_priv_by_default::foo::d>();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
    foo::<static_priv_by_default::foo::e>();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

