#![feature(specialization)]
#![feature(no_core)]
#![no_core]

trait Trait {
    fn unqualified_fn();
    unsafe fn unsafe_fn();
}

impl<T> Trait for T {
    default fn unqualified_fn() {}

    // `default` precedes all other qualifiers
    unsafe default fn unsafe_fn() {}
    // { dg-error "expecting .fn. but .identifier. found" "" { target *-*-* } .-1 }
    // { dg-error "expecting ... but .fn. found" "" { target *-*-* } .-2 }
    // { dg-error "failed to parse trait impl item in trait impl" "" { target *-*-* } .-3 }
}
