// [full] run-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

#[derive(PartialEq, Eq)]
struct NoMatch;

fn foo<const T: NoMatch>() -> bool {
// { dg-error "" "" { target *-*-* } .-1 }
    true
}

fn main() {
    foo::<{NoMatch}>();
}

