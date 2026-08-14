// The precise semantics of inhabitedness with respect to unions and references is currently
// undecided. This test file currently checks a conservative choice.

#![feature(exhaustive_patterns)]
#![feature(never_type)]

#![allow(dead_code)]
#![allow(unreachable_code)]

pub union Foo {
    foo: !,
}

fn uninhab_ref() -> &'static ! {
    unimplemented!()
}

fn uninhab_union() -> Foo {
    unimplemented!()
}

fn match_on_uninhab() {
    match uninhab_ref() {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    }

    match uninhab_union() {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
    }
}

fn main() {}

