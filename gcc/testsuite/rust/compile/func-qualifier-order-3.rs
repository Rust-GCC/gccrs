#![feature(min_specialization)]
#![feature(no_core)]
#![no_core]

trait Dummy {
}

// the purpose of this is to trigger the compiler message
// regarding the order of qualifiers
impl<T> Dummy for T {
    default async unsafe extern "C" const fn all_the_qualifiers() {}
    // { dg-error "invalid order of function qualifiers; found .default async unsafe extern const., expected .default const async unsafe extern." "" { target *-*-* } .-1 }
    // { dg-error "failed to parse trait impl item in trait impl" "" { target *-*-* } .-2 }
}
