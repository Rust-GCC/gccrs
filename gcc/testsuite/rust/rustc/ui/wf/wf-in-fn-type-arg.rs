// Check that we enforce WF conditions also for types in fns.

struct MustBeCopy<T:Copy> {
    t: T
}

struct Bar<T> {
    // needs T: Copy
    x: fn(MustBeCopy<T>) // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() { }

