#![warn(anonymous_parameters)]
// Test for the anonymous_parameters deprecation lint (RFC 1685)

// check-pass
// edition:2015
// run-rustfix

trait T {
    fn foo(i32); // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-2 }

    fn bar_with_default_impl(String, String) {}
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
}

fn main() {}

