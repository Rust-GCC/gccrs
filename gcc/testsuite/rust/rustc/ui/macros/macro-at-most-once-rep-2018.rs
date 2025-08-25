// Tests that `?` is a Kleene op and not a macro separator in the 2018 edition.

// edition:2018

macro_rules! foo {
    ($(a)?) => {};
}

// The Kleene op `?` does not admit a separator before it.
macro_rules! baz {
    ($(a),?) => {}; // { dg-error "" "" { target *-*-* } }
}

macro_rules! barplus {
    ($(a)?+) => {}; // ok. matches "a+" and "+"
}

macro_rules! barstar {
    ($(a)?*) => {}; // ok. matches "a*" and "*"
}

pub fn main() {
    foo!();
    foo!(a);
    foo!(a?); // { dg-error "" "" { target *-*-* } }
    foo!(a?a); // { dg-error "" "" { target *-*-* } }
    foo!(a?a?a); // { dg-error "" "" { target *-*-* } }

    barplus!(); // { dg-error "" "" { target *-*-* } }
    barplus!(a); // { dg-error "" "" { target *-*-* } }
    barplus!(a?); // { dg-error "" "" { target *-*-* } }
    barplus!(a?a); // { dg-error "" "" { target *-*-* } }
    barplus!(a+);
    barplus!(+);

    barstar!(); // { dg-error "" "" { target *-*-* } }
    barstar!(a); // { dg-error "" "" { target *-*-* } }
    barstar!(a?); // { dg-error "" "" { target *-*-* } }
    barstar!(a?a); // { dg-error "" "" { target *-*-* } }
    barstar!(a*);
    barstar!(*);
}

