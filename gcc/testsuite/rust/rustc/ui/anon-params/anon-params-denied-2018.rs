// Tests that anonymous parameters are a hard error in edition 2018.

// edition:2018

trait T {
    fn foo(i32); // { dg-error "" "" { target *-*-* } }

    fn bar_with_default_impl(String, String) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // do not complain about missing `b`
    fn baz(a:usize, b, c: usize) -> usize { // { dg-error "" "" { target *-*-* } }
        a + b + c
    }
}

fn main() {}

