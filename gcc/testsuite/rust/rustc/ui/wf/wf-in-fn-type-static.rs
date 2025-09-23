// Check that we enforce WF conditions related to regions also for
// types in fns.

#![allow(dead_code)]


struct MustBeCopy<T:Copy> {
    t: T
}

struct Foo<T> {
    // needs T: 'static
    x: fn() -> &'static T // { dg-error ".E0310." "" { target *-*-* } }
}

struct Bar<T> {
    // needs T: Copy
    x: fn(&'static T) // { dg-error ".E0310." "" { target *-*-* } }
}


fn main() { }

