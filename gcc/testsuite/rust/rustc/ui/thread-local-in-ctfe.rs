#![feature(const_fn, thread_local)]

#[thread_local]
static A: u32 = 1;

static B: u32 = A;
// { dg-error ".E0625." "" { target *-*-* } .-1 }

static C: &u32 = &A;
// { dg-error ".E0625." "" { target *-*-* } .-1 }

const D: u32 = A;
// { dg-error ".E0625." "" { target *-*-* } .-1 }

const E: &u32 = &A;
// { dg-error ".E0625." "" { target *-*-* } .-1 }

const fn f() -> u32 {
    A
// { dg-error ".E0625." "" { target *-*-* } .-1 }
}

fn main() {}

