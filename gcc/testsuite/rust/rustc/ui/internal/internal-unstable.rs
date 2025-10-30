// aux-build:internal_unstable.rs

#![feature(allow_internal_unstable)]

#[macro_use]
extern crate internal_unstable;

macro_rules! foo {
    ($e: expr, $f: expr) => {{
        $e;
        $f;
        internal_unstable::unstable(); // { dg-error ".E0658." "" { target *-*-* } }
    }}
}

#[allow_internal_unstable(function)]
macro_rules! bar {
    ($e: expr) => {{
        foo!($e,
             internal_unstable::unstable());
        internal_unstable::unstable();
    }}
}

fn main() {
    // ok, the instability is contained.
    call_unstable_allow!();
    construct_unstable_allow!(0);
    |x: internal_unstable::Foo| { call_method_allow!(x) };
    |x: internal_unstable::Bar| { access_field_allow!(x) };
    |x: internal_unstable::Bar| { access_field_allow2!(x) }; // regression test for #77088

    // bad.
    pass_through_allow!(internal_unstable::unstable()); // { dg-error ".E0658." "" { target *-*-* } }

    pass_through_noallow!(internal_unstable::unstable()); // { dg-error ".E0658." "" { target *-*-* } }



    println!("{:?}", internal_unstable::unstable()); // { dg-error ".E0658." "" { target *-*-* } }

    bar!(internal_unstable::unstable()); // { dg-error ".E0658." "" { target *-*-* } }
}

